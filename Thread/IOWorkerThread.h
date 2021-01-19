#pragma once

class IOWorkerThread {

public:
	IOWorkerThread(HANDLE hCompletionPort);
	~IOWorkerThread();

	void IOWorker();

private:
	bool mIsWorkerRun;
	HANDLE mCompletionPort;
};