#include "preprocessor.h"

CEREAL_REGISTER_TYPE(CTFOBaseEvent);
CEREAL_REGISTER_TYPE(LaunchEvent);
CEREAL_REGISTER_TYPE(FocusEvent);
CEREAL_REGISTER_TYPE(DeviceInfoEvent);
CEREAL_REGISTER_TYPE(CardActionEvent);

struct SimpleDumpJSONToStandardOutput {
  template <typename T>
  void operator()(const T& entry) {
    std::cout << JSON(entry) << std::endl;
  }
};

template <typename BASE_EVENT, typename CONSUMER>
struct DispatchToPreprocessor {
  EventPreprocessor<BASE_EVENT> preprocessor;
  CONSUMER consumer;

  template <typename T>
  void operator()(const T& entry) {
    preprocessor.DispatchLogEntry(entry, consumer);
  }
};

int main() {
  /*
    JSONByLineParser<LogEntry>::Process("test.txt",
                                        DispatchToPreprocessor<CTFOBaseEvent,
    SimpleDumpJSONToStandardOutput>());
    std::vector<std::string> v{"1", "2"};
    JSONByLineParser<LogEntry>::Process(v,
                                        DispatchToPreprocessor<CTFOBaseEvent,
    SimpleDumpJSONToStandardOutput>());
  */
  JSONByLineParser<LogEntry>::Process(std::cin,
                                      DispatchToPreprocessor<CTFOBaseEvent, SimpleDumpJSONToStandardOutput>());
}
