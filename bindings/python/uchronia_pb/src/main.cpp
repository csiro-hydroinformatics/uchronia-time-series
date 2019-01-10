#include <pybind11/pybind11.h>
#include "../include/uchronia_pb_exports.h"

#include "../include/cpp_bindings_generated.h"

#include <string>
#include <vector>

void delete_ansi_string_array(char** values, int arrayLength)
{
	DeleteAnsiStringArray(values, arrayLength);
}

void delete_array(double* values)
{
	DeleteDoubleArray(values);
}

namespace py = pybind11;

PYBIND11_MODULE(uchronia_pb, m) {

	//py::module::import("cinteroppyb11");

    m.doc() = R"pbdoc(
        Pybind11 uchronia bindings plugin
        -----------------------

        .. currentmodule:: uchronia_pb

        .. autosummary::
           :toctree: _generate

           CreateSingleTimeSeriesDataFromStruct
           etc.
    )pbdoc";
	m.def("GetLastStdExceptionMessage", &GetLastStdExceptionMessage_cpp, R"pbdoc( TODO doc for GetLastStdExceptionMessage)pbdoc");
	m.def("RegisterExceptionCallback", &RegisterExceptionCallback_cpp, R"pbdoc( TODO doc for RegisterExceptionCallback)pbdoc");
	m.def("DisposeSharedPointer", &DisposeSharedPointer_cpp, R"pbdoc( TODO doc for DisposeSharedPointer)pbdoc");
	m.def("SetTimeSeriesMissingValueValue", &SetTimeSeriesMissingValueValue_cpp, R"pbdoc( TODO doc for SetTimeSeriesMissingValueValue)pbdoc");
	m.def("LoadEnsembleDataset", &LoadEnsembleDataset_cpp, R"pbdoc( TODO doc for LoadEnsembleDataset)pbdoc");
	m.def("CreateEnsembleDataset", &CreateEnsembleDataset_cpp, R"pbdoc( TODO doc for CreateEnsembleDataset)pbdoc");
	m.def("GetEnsembleDatasetDataIdentifiers", &GetEnsembleDatasetDataIdentifiers_cpp, R"pbdoc( TODO doc for GetEnsembleDatasetDataIdentifiers)pbdoc");
	m.def("GetEnsembleDatasetDataSubIdentifiers", &GetEnsembleDatasetDataSubIdentifiers_cpp, R"pbdoc( TODO doc for GetEnsembleDatasetDataSubIdentifiers)pbdoc");
	m.def("GetEnsembleDatasetDataSummaries", &GetEnsembleDatasetDataSummaries_cpp, R"pbdoc( TODO doc for GetEnsembleDatasetDataSummaries)pbdoc");
	m.def("EnsembleSizeEnsembleTimeSeries", &EnsembleSizeEnsembleTimeSeries_cpp, R"pbdoc( TODO doc for EnsembleSizeEnsembleTimeSeries)pbdoc");
	m.def("CreateEnsembleForecastTimeSeries", &CreateEnsembleForecastTimeSeries_cpp, R"pbdoc( TODO doc for CreateEnsembleForecastTimeSeries)pbdoc");
	m.def("GetDatasetSingleTimeSeries", &GetDatasetSingleTimeSeries_cpp, R"pbdoc( TODO doc for GetDatasetSingleTimeSeries)pbdoc");
	m.def("GetDatasetEnsembleTimeSeries", &GetDatasetEnsembleTimeSeries_cpp, R"pbdoc( TODO doc for GetDatasetEnsembleTimeSeries)pbdoc");
	m.def("GetDatasetEnsembleForecastTimeSeries", &GetDatasetEnsembleForecastTimeSeries_cpp, R"pbdoc( TODO doc for GetDatasetEnsembleForecastTimeSeries)pbdoc");
	m.def("SaveSingleTimeSeriesToNetcdf", &SaveSingleTimeSeriesToNetcdf_cpp, R"pbdoc( TODO doc for SaveSingleTimeSeriesToNetcdf)pbdoc");
	m.def("SaveEnsembleTimeSeriesToNetcdf", &SaveEnsembleTimeSeriesToNetcdf_cpp, R"pbdoc( TODO doc for SaveEnsembleTimeSeriesToNetcdf)pbdoc");
	m.def("SaveEnsembleForecastTimeSeriesToNetcdf", &SaveEnsembleForecastTimeSeriesToNetcdf_cpp, R"pbdoc( TODO doc for SaveEnsembleForecastTimeSeriesToNetcdf)pbdoc");
	m.def("IsMissingValueItemEnsembleForecastTimeSeries", &IsMissingValueItemEnsembleForecastTimeSeries_cpp, R"pbdoc( TODO doc for IsMissingValueItemEnsembleForecastTimeSeries)pbdoc");
	m.def("GetItemEnsembleForecastTimeSeries", &GetItemEnsembleForecastTimeSeries_cpp, R"pbdoc( TODO doc for GetItemEnsembleForecastTimeSeries)pbdoc");
	m.def("TimeSeriesFromEnsembleOfTimeSeries", &TimeSeriesFromEnsembleOfTimeSeries_cpp, R"pbdoc( TODO doc for TimeSeriesFromEnsembleOfTimeSeries)pbdoc");
	m.def("TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries", &TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_cpp, R"pbdoc( TODO doc for TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries)pbdoc");
	m.def("GetValueFromUnivariateTimeSeries", &GetValueFromUnivariateTimeSeries_cpp, R"pbdoc( TODO doc for GetValueFromUnivariateTimeSeries)pbdoc");
	m.def("TransformEachItem", &TransformEachItem_cpp, R"pbdoc( TODO doc for TransformEachItem)pbdoc");
	m.def("SetValueToUnivariateTimeSeries", &SetValueToUnivariateTimeSeries_cpp, R"pbdoc( TODO doc for SetValueToUnivariateTimeSeries)pbdoc");
	m.def("GetItemEnsembleForecastTimeSeriesAsStructure", &GetItemEnsembleForecastTimeSeriesAsStructure_cpp, R"pbdoc( TODO doc for GetItemEnsembleForecastTimeSeriesAsStructure)pbdoc");
	m.def("GetItemEnsembleTimeSeriesAsStructure", &GetItemEnsembleTimeSeriesAsStructure_cpp, R"pbdoc( TODO doc for GetItemEnsembleTimeSeriesAsStructure)pbdoc");
	m.def("SetItemEnsembleForecastTimeSeriesAsStructure", &SetItemEnsembleForecastTimeSeriesAsStructure_cpp, R"pbdoc( TODO doc for SetItemEnsembleForecastTimeSeriesAsStructure)pbdoc");
	m.def("SetItemEnsembleTimeSeriesAsStructure", &SetItemEnsembleTimeSeriesAsStructure_cpp, R"pbdoc( TODO doc for SetItemEnsembleTimeSeriesAsStructure)pbdoc");
	m.def("CreatePerfectForecastTimeSeries", &CreatePerfectForecastTimeSeries_cpp, R"pbdoc( TODO doc for CreatePerfectForecastTimeSeries)pbdoc");
	m.def("ToStructEnsembleTimeSeriesData", &ToStructEnsembleTimeSeriesData_cpp, R"pbdoc( TODO doc for ToStructEnsembleTimeSeriesData)pbdoc");
	m.def("ToStructSingleTimeSeriesData", &ToStructSingleTimeSeriesData_cpp, R"pbdoc( TODO doc for ToStructSingleTimeSeriesData)pbdoc");
	m.def("CreateEnsembleTimeSeriesDataFromStruct", &CreateEnsembleTimeSeriesDataFromStruct_cpp, R"pbdoc( TODO doc for CreateEnsembleTimeSeriesDataFromStruct)pbdoc");
	m.def("CreateSingleTimeSeriesDataFromStruct", &CreateSingleTimeSeriesDataFromStruct_cpp, R"pbdoc( TODO doc for CreateSingleTimeSeriesDataFromStruct)pbdoc");
	m.def("GetTimeSeriesGeometry", &GetTimeSeriesGeometry_cpp, R"pbdoc( TODO doc for GetTimeSeriesGeometry)pbdoc");
	m.def("GetEnsembleForecastTimeSeriesGeometry", &GetEnsembleForecastTimeSeriesGeometry_cpp, R"pbdoc( TODO doc for GetEnsembleForecastTimeSeriesGeometry)pbdoc");
	m.def("GetNumTimeSeries", &GetNumTimeSeries_cpp, R"pbdoc( TODO doc for GetNumTimeSeries)pbdoc");
	m.def("GetProviderTsGeometry", &GetProviderTsGeometry_cpp, R"pbdoc( TODO doc for GetProviderTsGeometry)pbdoc");
	m.def("GetProviderTimeSeriesIdentifiers", &GetProviderTimeSeriesIdentifiers_cpp, R"pbdoc( TODO doc for GetProviderTimeSeriesIdentifiers)pbdoc");
	m.def("TimeSeriesFromProviderTs", &TimeSeriesFromProviderTs_cpp, R"pbdoc( TODO doc for TimeSeriesFromProviderTs)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
