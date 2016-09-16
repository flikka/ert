from ert.enkf import ENKF_LIB
from ert.enkf.ert_run_context import ErtRunContext
from ert.enkf.run_arg import RunArg
from ert.job_queue import JobQueueManager
from ert.util import BoolVector, ArgPack, CThreadPool


class SimulationContext(object):
    def __init__(self, ert, size, verbose=False):
        self._ert = ert
        """ :type: ert.enkf.EnKFMain """
        self._size = size
        max_runtime = ert.analysisConfig().get_max_runtime()
        print("Max runtime is {}".format(max_runtime))
        job_queue = ert.siteConfig().getJobQueue().set_max_job_duration(max_runtime)

        self._queue_manager = JobQueueManager(job_queue)
        self._queue_manager.startQueue(size, verbose=verbose)
        self._run_args = {}
        """ :type: dict[int, RunArg] """

        self._thread_pool = CThreadPool(8)
        self._thread_pool.addTaskFunction("submitJob", ENKF_LIB, "enkf_main_isubmit_job__")


    def addSimulation(self, iens, target_fs):
        if iens >= self._size:
            raise UserWarning("Realization number out of range: %d >= %d" % (iens, self._size))

        if iens in self._run_args:
            raise UserWarning("Realization number: '%d' already queued" % iens)

        runpath_fmt = self._ert.getModelConfig().getRunpathFormat()
        member = self._ert.getRealisation(iens)
        runpath = ErtRunContext.createRunpath(iens , runpath_fmt, member.getDataKW( ))
        run_arg = RunArg.createEnsembleExperimentRunArg(target_fs, iens, runpath)

        self._ert.createRunPath(run_arg)

        self._run_args[iens] = run_arg
        self._thread_pool.submitJob(ArgPack(self._ert, run_arg))


    def isRunning(self):
        return self._queue_manager.isRunning()


    def getNumRunning(self):
        return self._queue_manager.getNumRunning()


    def getNumSuccess(self):
        return self._queue_manager.getNumSuccess()


    def getNumFailed(self):
        return self._queue_manager.getNumFailed()

    def getNumWaiting(self):
        return self._queue_manager.getNumWaiting()


    def didRealizationSucceed(self, iens):
        queue_index = self._run_args[iens].getQueueIndex()
        return self._queue_manager.didJobSucceed(queue_index)


    def didRealizationFail(self, iens):
        queue_index = self._run_args[iens].getQueueIndex()
        return self._queue_manager.didJobFail(queue_index)


    def isRealizationQueued(self, iens):
        return iens in self._run_args


    def isRealizationFinished(self, iens):
        run_arg = self._run_args[iens]

        if run_arg.isSubmitted():
            queue_index = run_arg.getQueueIndex()
            return self._queue_manager.isJobComplete(queue_index)
        else:
            return False

