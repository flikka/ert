set(TEST_SOURCES
    __init__.py
    test_simulations_tracker.py
)

add_python_package("python.tests.gui.simulation.models" ${PYTHON_INSTALL_PREFIX}/tests/gui/simulation/models "${TEST_SOURCES}" False)

addPythonTest(gui.simulation.models.simulations_tracker tests.gui.simulation.models.test_simulations_tracker.SimulationsTrackerTest)