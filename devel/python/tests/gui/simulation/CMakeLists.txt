set(TEST_SOURCES
    __init__.py
)

add_python_package("python.tests.gui.simulation" ${PYTHON_INSTALL_PREFIX}/tests/gui/simulation "${TEST_SOURCES}" False)
add_subdirectory(models)