from ert.test import ExtendedTestCase
from ert_gui.simulation.models import SimulationsTracker


class SimulationsTrackerTest(ExtendedTestCase):
    def test_construct(self):
        simulations_tracker = SimulationsTracker()
        self.assertIsNotNone(simulations_tracker)


