from ert.test import ExtendedTestCase
from ert_gui.simulation.models import SimulationsTracker


class SimulationsTrackerTest(ExtendedTestCase):
    def test_construct(self):
        simulations_tracker = SimulationsTracker()
        self.assertIsNotNone(simulations_tracker)

    def test_getStatus(self):
        simulations_tracker = SimulationsTracker()
        states = simulations_tracker.getStates()
        self.assertEqual(len(states), 5)

    def test_numFinishedSimulations(self):
        simulations_tracker = SimulationsTracker()
        state_done = simulations_tracker.getStates()[4]
        self.assertEqual(state_done.name, "Finished")
        self.assertEqual(simulations_tracker.numFinishedSimulations(), 0)
        state_done.count = 2
        self.assertEqual(simulations_tracker.numFinishedSimulations(), 2)


