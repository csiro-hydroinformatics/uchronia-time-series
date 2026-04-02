import os

from uchronia.data_set import get_ensemble_dataset


def sample_data_dir(do_warn=True):
    """
    Probe for the location of sample data directory (too big for package inclusion)

    Probe for the location of sample data directory (too big for package inclusion), defined by the SWIFT_SAMPLE_DATA_DIR environment variable.

    Args:
        doWarn (bool): issue a warning if SWIFT_SAMPLE_DATA_DIR env var is not set

    Returns:
        path to the sample data directory

    """
    d = os.environ["SWIFT_SAMPLE_DATA_DIR"]
    if do_warn:
        if not os.path.exists(d):
            raise Exception(
                "Non-existent environment variable 'SWIFT_SAMPLE_DATA_DIR', or its value is not an existing directory"
            )
    return d


#' @export
def has_sample_data():
    d = sample_data_dir(do_warn=False)
    return os.path.exists(d)


def sample_time_series_library(ident: str):
    """
    Get one of the sample time series libraries included in this package

    Get one of the sample time series libraries included in this package

    Args:
        ident (Any): an identifier for the library. Choice (case insensitive): 'Upper Murray', 'Stanley'

    Returns:
        a time series library

    """
    ident = ident.lower()
    assert ident in ["upper murray", "stanley"]
    doc_data_path = sample_data_dir()

    here = os.path.abspath(os.path.dirname(__file__))
    pkg_dir = os.path.join(here, ".")
    pkg_data_dir = os.path.join(pkg_dir, "data")
    if ident == "upper murray":
        data_library_file = os.path.join(
            pkg_data_dir, "time_series_library_upper_murray.yaml"
        )
        data_path = os.path.join(doc_data_path, "UpperMurray")
    elif ident == "stanley":
        data_library_file = os.path.join(
            pkg_data_dir, "time_series_library_stanley.yaml"
        )
        data_path = os.path.join(doc_data_path, "RPP")
    if not os.path.exists(data_path):
        raise FileNotFoundError("directory path expected but not found: " + data_path)
    data_library = get_ensemble_dataset(data_library_file, data_path)
    return data_library
