class Task {
private:
  int value;
  int priority;
  long long durationMs;
  int steps;
  int currentStep;

public:
  Task();
  int getValue();
  int getPriority();
  long long getDurationMs();
  int getSteps();
  int getCurrentStep();
  void run();
};