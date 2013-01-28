#ifndef TaskManager_h__
#define TaskManager_h__

/// -----------------------------------------------------------------------
class TaskManager {
public:
	TaskManager();
	~TaskManager();

	void doTask(int argc, _TCHAR* argv[]);

private:
	void displayUsage();
};

#endif // TaskManager_h__