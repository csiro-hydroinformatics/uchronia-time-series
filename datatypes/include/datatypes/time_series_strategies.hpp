#pragma once


//#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/function.hpp>
#include "datatypes/common.h"
#include "datatypes/exception_utilities.h"

#include <iterator>

namespace datatypes
{
	namespace timeseries
	{
		/**
		 * \class	StoragePolicy
		 *
		 * \brief	An interface for classes that can handle the storage of data for time series.
		 *
		 *          The storage of time series data is using a software pattern usually named Strategy,
		 *          or Policy. The word "policy" is more of a legacy (time series storage used to be 
		 *          template-arguments for time series). The purpose is to delegate the details of the 
		 *          data handling (memory, file, and data caching between different types of storages.) 
		 *          out of TTimeSeries objects.
		 *
		 * \tparam	T	The type of each data item this can handle, e.g. a double, float, or even pointers.
		 */
		template <typename T>
		class StoragePolicy :
			public TimeSeriesInfoProvider
		{
		protected:
			StoragePolicy(const StoragePolicy& src) 
			{
			}
			StoragePolicy() {}
		public:
			virtual ~StoragePolicy() {}
			virtual void CopyTo(vector<T>& dest, size_t from = 0, size_t to = -1) const = 0;
			virtual size_t Size() const = 0;
			virtual void Allocate(size_t length, T value) = 0;
			virtual void AllocateValues(size_t length, const T* values) = 0;
			virtual void AllocateValues(const vector<T>& values) = 0;
			virtual const T& operator[](const size_t i) const = 0;
			virtual T& operator[](const size_t i) = 0;
			virtual StoragePolicy* Clone() const = 0;

			virtual void CopyTo(StoragePolicy<T>& dest)
			{
				vector<T> x;
				this->CopyTo(x);
				dest.SetStart(this->GetStart());
				dest.SetTimeStep(this->GetTimeStep());
				dest.AllocateValues(x);
			}

			virtual bool ReadOnly() { return false; }

			virtual TimeStep GetTimeStep() const = 0;
			virtual ptime GetStart() const = 0;
			virtual void SetTimeStep(const TimeStep& tStep) = 0;
			virtual void SetStart(const ptime & start) = 0;

		};

		/**
		 * \class	MissingValuePolicy
		 *
		 * \brief	An interface for classes that define missing values in time series.
		 *
		 * \tparam	T	Generic type parameter.
		 */

		template <typename T>
		class MissingValuePolicy
		{
		public:
			virtual ~MissingValuePolicy() {}
			virtual bool IsMissingValue(const T& a) const = 0;
			virtual T GetMissingValue() const = 0;
			virtual MissingValuePolicy* Clone() const = 0;
		};

		template <typename T = double>
		class DefaultMissingFloatingPointPolicy
			: public MissingValuePolicy<T>
		{
		private:
			const T missingValue = (T)(DEFAULT_MISSING_DATA_VALUE);
		public:
			inline bool IsMissingValue(const T& a) const { return (a == missingValue); };
			inline T GetMissingValue() const { return missingValue; };
			MissingValuePolicy<T>* Clone() const { return new DefaultMissingFloatingPointPolicy<T>(); };
		};

		template <typename T>
		class NullPointerIsMissingPolicy
			: public MissingValuePolicy<T>
		{
		public:
			inline bool IsMissingValue(const T& a) const { return (a == nullptr); };
			inline T GetMissingValue() const { return nullptr; };
			MissingValuePolicy<T>* Clone() const { return new NullPointerIsMissingPolicy<T>(); };
		};


		template <typename T = double>
		class NegativeIsMissingFloadingPointPolicy
			: public MissingValuePolicy<T>
		{
		private:
			const T missingValue = (T)(DEFAULT_MISSING_DATA_VALUE);
		public:
			inline bool IsMissingValue(const T& a) const { return (a < T()); };
			inline T GetMissingValue() const { return missingValue; };
			MissingValuePolicy<T>* Clone() const { return new NegativeIsMissingFloadingPointPolicy<T>(); };
		};


		template <typename T = double>
		class StlVectorStorage
			: public StoragePolicy<T>
		{
		private:
			StlVectorStorage(const StlVectorStorage& src) :
				StoragePolicy<T>(src)
			{
				data = src.data;
				startDate = src.startDate;
				timeStep = src.timeStep;
			}
			vector<T> data;
			void CheckIntervalBounds(const size_t& from, size_t& to) const
			{
				size_t tsLen = this->Size();
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesInterval(from, to, tsLen);
			}

		private:
			// the temporal information for the time series must be stored here 
			// to support time series with shared storages. Conceptually not quite satisfying, 
			// but preferable to shared values with different time series with different temporal
			// characteristics, for now.
			ptime startDate;
			TimeStep timeStep;

		public:
			StlVectorStorage()
			{
			}

			size_t Size() const { return data.size(); }

			size_t GetLength() const
			{
				return Size();
			}

			void Allocate(size_t length, T value)
			{
				data.clear();
				data.reserve(length);
				data.assign(length, value);
			}
			void AllocateValues(size_t length, const T* values)
			{
				data.clear();
				data.reserve(length);
				data.assign(values, values + length);
			}
			void AllocateValues(const vector<T>& values)
			{
				data.clear();
				data.reserve(values.size());
				data.assign(values.begin(), values.end());
			}

			void CopyTo(vector<T>& dest, size_t from = 0, size_t to = -1) const
			{
				CheckIntervalBounds(from, to);
				size_t len = (to - from) + 1;
				if (dest.size() != len)
				{
					dest.clear();
					dest.resize(len);
				};
				std::copy(data.begin() + from, data.begin() + to + 1, dest.begin());
			}

			T& operator[](const size_t i) {
				// Add a check on index bounds.
				// If performance requires it, we might restrict this check based on whether T is a class or a numeric type
				// in a manner similar to DefaultMissingValuePolicyTypeFactory. But prefer safety.
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, data.size(), "i");
				T& vRef = data[i];
				return vRef;
			}
			const T& operator[](const size_t i) const {
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, data.size(), "i");
				return data[i];
			}
			StoragePolicy<T>* Clone() const { return new StlVectorStorage<T>(*this); }

			TimeStep GetTimeStep() const override
			{
				return timeStep;
			}

			ptime GetStart() const override
			{
				return startDate;
			}

			void SetTimeStep(const TimeStep& tStep) override
			{
				timeStep = tStep;
			}

			void SetStart(const ptime & start) override
			{
				startDate = start;
			}


		};

		/**
		 * \class	SharedVectorStorage
		 *
		 * \brief	A storage strategy for time serie such that data is a shared state amongst several time series
		 *
		 * \tparam	T	The type of the elements in the time series.
		 */

		template <typename T = double>
		class SharedVectorStorage
			: public StoragePolicy<T>
		{
		private:
			SharedVectorStorage(const SharedVectorStorage& src)
			{
				d = src.d;
			}
			class SharedData
			{
			public:
				vector<T> data;
				ptime startDate;
				TimeStep timeStep;
			};
			using SD = typename SharedVectorStorage<T>::SharedData;

			std::shared_ptr<SD> d;

			void CheckIntervalBounds(const size_t& from, size_t& to) const
			{
				size_t tsLen = this->Size();
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesInterval(from, to, tsLen);
			}
		public:
			SharedVectorStorage()
			{
				d = std::shared_ptr<SD>(new SD());
			}

			size_t Size() const { return d->data.size(); }
			void Allocate(size_t length, T value)
			{
				d->data.clear();
				d->data.reserve(length);
				d->data.assign(length, value);
			}
			void AllocateValues(size_t length, const T* values)
			{
				d->data.clear();
				d->data.reserve(length);
				d->data.assign(values, values + length);
			}
			void AllocateValues(const vector<T>& values)
			{
				d->data.clear();
				d->data.reserve(values.size());
				d->data.assign(values.begin(), values.end());
			}

			void CopyTo(vector<T>& dest, size_t from = 0, size_t to = -1) const
			{
				size_t tsLen = this->Size();
				if (tsLen == 0)
					return;
				CheckIntervalBounds(from, to);
				size_t len = (to - from) + 1;
				if (dest.size() != len)
				{
					dest.clear();
					dest.resize(len);
				};
				std::copy(d->data.begin() + from, d->data.begin() + to + 1, dest.begin());
			}

			T& operator[](const size_t i) {
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, (d->data).size(), "i");
				T& vRef = (d->data)[i];
				return vRef;
			}
			const T& operator[](const size_t i) const {
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, (d->data).size(), "i");
				return (d->data)[i];
			}
			StoragePolicy<T>* Clone() const { return new SharedVectorStorage<T>(*this); }

			size_t GetLength() const
			{
				return Size();
			}

			TimeStep GetTimeStep() const override
			{
				return d->timeStep;
			}

			ptime GetStart() const override
			{
				return d->startDate;
			}

			void SetTimeStep(const TimeStep& tStep) override
			{
				d->timeStep = tStep;
			}

			void SetStart(const ptime & start) override
			{
				d->startDate = start;
			}

		};

		template <typename T = double>
		class MemoryCachingStorageWriter
			: public StoragePolicy<T>
		{
		protected:
			MemoryCachingStorageWriter(const MemoryCachingStorageWriter& src) :
				StoragePolicy<T>(src)
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("MemoryCachingStorageWriter", "copy ctor");
			}
		private:

			vector<T> buffer;
			size_t bufferSize;
			size_t bufferPosition;
			void CheckIntervalBounds(const size_t& from, size_t& to) const
			{
				size_t tsLen = this->Size();
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesInterval(from, to, tsLen);
			}

			StoragePolicy<T>* wrappedStorage;

		public:
			MemoryCachingStorageWriter(size_t bufferSize, StoragePolicy<T>* wrappedStorage) :
				StoragePolicy<T>(),
				bufferSize(bufferSize)
			{
				if (wrappedStorage == nullptr) datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("wrappedStorage must not be a nullptr");
				buffer = vector<T>(bufferSize);
				bufferPosition = 0;
				this->wrappedStorage = wrappedStorage;
			}

			T& GetWindowItem(size_t i)
			{
				return buffer[i];
			}

			T& GetBackendItem(size_t i)
			{
				return (*wrappedStorage).operator[](i);
			}

			size_t Size() const { return wrappedStorage->Size(); }

			size_t GetLength() const
			{
				return Size();
			}

			TimeStep GetTimeStep() const override
			{
				return wrappedStorage->GetTimeStep();
			}

			ptime GetStart() const override
			{
				return wrappedStorage->GetStart();
			}

			void SetTimeStep(const TimeStep& tStep) override
			{
				wrappedStorage->SetTimeStep(tStep);
			}

			void SetStart(const ptime & start) override
			{
				wrappedStorage->SetStart(start);
			}

			void ResetBuffer()
			{
				using U = typename datatypes::utils::DisposeVectorTypeFactory<T>::type;
				U::Dispose(buffer);
			}

			void Allocate(size_t length, T value)
			{
				ResetBuffer();
				buffer = vector<T>(bufferSize);
				bufferPosition = 0;
				wrappedStorage->Allocate(length, value);
			}

			void AllocateValues(size_t length, const T* values)
			{
				//datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("MemoryCachingStorageWriter", "AllocateValues(size_t length, const T* values)");
				ResetBuffer();
				buffer = vector<T>(bufferSize);
				bufferPosition = 0;
				wrappedStorage->AllocateValues(length, values);
			}

			void AllocateValues(const vector<T>& values)
			{
				ResetBuffer();
				buffer = vector<T>(bufferSize);
				bufferPosition = 0;
				wrappedStorage->AllocateValues(values);
			}

			void CopyTo(vector<T>& dest, size_t from = 0, size_t to = -1) const
			{
				wrappedStorage->CopyTo(dest, from, to);
			}

		private:
			void shiftWindow(size_t i)
			{
				size_t n = std::min(bufferSize, wrappedStorage->Size() - bufferPosition);
				for (size_t j = 0; j < bufferSize; j++)
				{
					(*wrappedStorage).operator[](bufferPosition + j) = buffer[j];
				}
				//std::copy(buffer.begin(), buffer.begin() + n, data.begin() + bufferPosition);
				buffer = vector<T>(bufferSize);
				bufferPosition = i / bufferSize;
			}

		public:
			T& operator[](const size_t i) {
				// Add a check on index bounds.
				// If performance requires it, we might restrict this check based on whether T is a class or a numeric type
				// in a manner similar to DefaultMissingValuePolicyTypeFactory. But prefer safety.
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, wrappedStorage->Size(), "i");
				if (i > (this->bufferPosition + this->bufferSize - 1))
					shiftWindow(i);
				T& vRef = buffer[i % bufferSize];
				return vRef;
			}
			const T& operator[](const size_t i) const {
				datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("MemoryCachingStorageWriter", "AllocateValues(size_t length, const T* values)");
				//datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesIndex(i, wrappedStorage->Size(), "i");
				//return data[i];
				return buffer[-1];
			}
			StoragePolicy<T>* Clone() const { return new MemoryCachingStorageWriter<T>(*this); }

		};


		template <typename TsType>
		class EnsembleStoragePolicy
		{
		public:

			typedef typename std::remove_pointer<TsType>::type Type;
			typedef typename std::add_pointer<Type>::type PtrType;
			typedef typename Type::ElementType ElementType;

			EnsembleStoragePolicy() {}
			virtual ~EnsembleStoragePolicy() {}

			virtual void Reset(const vector<PtrType>& values, const ptime& startDate, const TimeStep& timeStep) = 0;
			EnsembleStoragePolicy& operator=(const EnsembleStoragePolicy& src) {
				if (&src == this) {
					return *this;
				}
				OperatorEqualImpl(src);
				return *this;
			}

			// ?? virtual EnsembleStoragePolicy& operator=(EnsembleStoragePolicy&& src) = 0;
			virtual void ResetSeries(const size_t& numSeries, const size_t& lengthSeries, const ptime& startDate, const TimeStep& timeStep) = 0;
			virtual TsType Get(size_t i) = 0;
			virtual ElementType Get(size_t i, size_t tsIndex) = 0;
			virtual void Set(size_t i, size_t tsIndex, ElementType val) = 0;
			virtual void Set(size_t i, const Type& val) = 0;
			virtual vector<ElementType*>* GetValues() const = 0;
			virtual void CopyTo(ElementType ** dest) const = 0;
			virtual size_t Size() const = 0;
			virtual size_t GetLength(size_t i) const = 0;
			//virtual ptime GetStartDate() const = 0;
			//virtual TimeStep GetTimeStep() const = 0;
			virtual void Clear() = 0;
			virtual const vector<PtrType>& AsReadonlyVector() const = 0;
			virtual EnsembleStoragePolicy<TsType>* Clone() const = 0;

		protected:
			virtual void OperatorEqualImpl(const EnsembleStoragePolicy<TsType>& src) = 0;
		};

		template <typename TsType>
		class StdVectorEnsembleStoragePolicy : public EnsembleStoragePolicy<TsType>
		{

		public:

			typedef typename std::remove_pointer<TsType>::type Type;
			typedef typename std::add_pointer<Type>::type PtrType;
			typedef typename Type::ElementType ElementType;

		private:

			vector<PtrType> series;

		public:
			void Reset(const vector<PtrType>& values, const ptime& startDate, const TimeStep& timeStep)
			{
				Clear();
				for (const PtrType d : values)
				{
					series.push_back(new Type(*d));
				}
			}

			StdVectorEnsembleStoragePolicy(const StdVectorEnsembleStoragePolicy<TsType>& src)
			{
				DeepCopyFrom(src);
			}

			StdVectorEnsembleStoragePolicy()
			{
			}

			~StdVectorEnsembleStoragePolicy()
			{
				Clear();
			}

			StdVectorEnsembleStoragePolicy& operator=(StdVectorEnsembleStoragePolicy&& src) {
				if (&src == this) {
					return *this;
				}
				std::swap(series, src.series);
				return *this;
			}

			void ResetSeries(const size_t& numSeries, const size_t& lengthSeries, const ptime& startDate, const TimeStep& timeStep)
			{
				Clear();
				for (size_t i = 0; i < numSeries; i++)
				{
					series.push_back(new Type(lengthSeries, startDate, timeStep));
				}
			}

			TsType Get(size_t i)
			{
				return TsType(series.at(i));
			}

			Type Get(size_t i) const
			{
				return Type(series.at(i));
			}

			ElementType Get(size_t i, size_t tsIndex)
			{
				return (*series[i])[tsIndex];
			}

			void Set(size_t i, size_t tsIndex, ElementType val)
			{
				PtrType a = series.at(i);
				(*a)[tsIndex] = val;
			}

			void Set(size_t i, const Type& val)
			{
				PtrType ts = series[i];
				*ts = val; // overloaded operator= should do this as expected - tbc
			}

			vector<ElementType*>* GetValues() const
			{
				vector<ElementType*>* result = new vector<ElementType*>();
				for (auto& d : series)
				{
					result->push_back(d->GetValues());
				}
				return result;
			}

			void CopyTo(ElementType ** dest) const
			{
				for (size_t i = 0; i < series.size(); i++)
				{
					dest[i] = series[i]->GetValues();
				}
			}

			size_t Size() const
			{
				return this->series.size();
			}

			size_t GetLength(size_t i) const
			{
				return this->series[i]->GetLength();
			}

			void Clear()
			{
				for (auto& d : series)
				{
					if (d != nullptr) delete d;
				}
				series.clear();
			}

			const vector<PtrType>& AsReadonlyVector() const
			{
				return series;
			}

			EnsembleStoragePolicy<TsType>* Clone() const { return new StdVectorEnsembleStoragePolicy<TsType>(*this); }

		private:

			void DeepCopyFrom(const EnsembleStoragePolicy<TsType>& src)
			{
				Clear();
				auto v = src.AsReadonlyVector();
				for (size_t i = 0; i < v.size(); i++)
				{
					Type* copy = new Type(*(v.at(i)));
					this->series.push_back(copy);
				}
			}

		protected:
			void OperatorEqualImpl(const EnsembleStoragePolicy<TsType>& src)
			{
				DeepCopyFrom(src);
			}

		};



	}
}
