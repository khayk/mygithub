#ifndef __taskManager_h__
#define __taskManager_h__



/// -----------------------------------------------------------------------
class TaskManager {
public:
	TaskManager();
	~TaskManager();

	void doTask(int argc, _TCHAR* argv[]);

private:
	void displayUsage();
	//string_t taskTodo_;

	std::vector<tCmdSp> cmds_;
};

#endif
