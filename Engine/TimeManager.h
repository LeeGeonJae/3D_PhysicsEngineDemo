
	class TimeManager
	{
	private:
		TimeManager();

	public:
		static TimeManager* GetInstance()
		{
			static TimeManager timeManager;
			return &timeManager;
		}

	public:

	};