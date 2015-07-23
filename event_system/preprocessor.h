#include "event_schema.h"
#include "../util.h"

#include "../../Current/Bricks/strings/is_string_type.h"
#include "../../Current/Bricks/template/metaprogramming.h"
#include "../../Current/EventCollector/event_collector.h"
#include "../../Current/Midichlorians/Dev/Beta/MidichloriansDataDictionary.h"

template <typename T>
struct JSONByLineParser {
  template <typename F>
  static void Process(const std::string& filename, F&& f) {
    std::ifstream is(filename);
    // Test for failbit & throw?
    JSONByLineParser<T>::Process(is, std::forward<F>(f));
  }

  template <typename F>
  static void Process(std::istream& is, F&& f) {
    std::string line;
    while (std::getline(is, line)) {
      ParseJSONAndCallF(line, std::forward<F>(f));
    }
  }

  template <typename C, typename F>
  static typename std::enable_if<!bricks::strings::is_string_type<C>::value &&
                                 std::is_convertible<typename C::value_type, std::string>::value>::type
  Process(const C& container, F&& f) {
    for (const auto& cit : container) {
      ParseJSONAndCallF(cit, std::forward<F>(f));
    }
  }

 private:
  template <typename F>
  static void ParseJSONAndCallF(const std::string& line, F&& f) {
    T object;
    try {
      ParseJSON(line, object);
      f(object);
    } catch (const bricks::ParseJSONException&) {
      std::cerr << "Unable to parse JSON." << std::endl;
    }
  }
};

template <typename BASE_EVENT>
struct MidichloriansEventPreprocessor {
  typedef BASE_EVENT T_BASE_EVENT;
  typedef std::unique_ptr<BASE_EVENT> T_RETURN_TYPE;
  typedef std::tuple<iOSDeviceInfo, iOSAppLaunchEvent, iOSFirstLaunchEvent, iOSFocusEvent, iOSGenericEvent>
      T_SUPPORTED_TYPES;

  template <typename F>
  void operator()(const iOSDeviceInfo& source_event, uint64_t t, F&& f) {
    DeviceInfoEvent device_info_event;
    ExtractBaseEventFields(source_event, t, device_info_event);
    device_info_event.model = source_event.info.at("deviceModel");
    device_info_event.name = source_event.info.at("deviceName");
    f(std::move(device_info_event));
  }

  template <typename F>
  void operator()(const iOSAppLaunchEvent& source_event, uint64_t t, F&& f) {
    LaunchEvent launch_event(false);
    ExtractBaseEventFields(source_event, t, launch_event);
    f(std::move(launch_event));
  }

  template <typename F>
  void operator()(const iOSFirstLaunchEvent& source_event, uint64_t t, F&& f) {
    LaunchEvent launch_event(true);
    ExtractBaseEventFields(source_event, t, launch_event);
    f(std::move(launch_event));
  }

  template <typename F>
  void operator()(const iOSFocusEvent& source_event, uint64_t t, F&& f) {
    FocusEvent focus_event(source_event.gained_focus);
    ExtractBaseEventFields(source_event, t, focus_event);
    f(std::move(focus_event));
  }

  template <typename F>
  void operator()(const iOSGenericEvent& source_event, uint64_t t, F&& f) {
    const std::map<std::string, ANSWER> card_actions = {
        {"CTFO", ANSWER::CTFO}, {"TFU", ANSWER::TFU}, {"SKIP", ANSWER::SKIP}};
    const std::map<std::string, FeedSwitchEvent::FEED> feeds = {{"Hot", FeedSwitchEvent::FEED::Hot},
                                                                {"Recent", FeedSwitchEvent::FEED::Recent}};

    try {
      const ANSWER action = card_actions.at(source_event.event);
      const UID uid = StringToUID(source_event.fields.at("uid"));
      const CID cid = StringToCID(source_event.fields.at("cid"));
      CardActionEvent card_action_event;
      ExtractBaseEventFields(source_event, t, card_action_event);
      card_action_event.uid = uid;
      card_action_event.cid = cid;
      card_action_event.action = action;
      f(std::move(card_action_event));
      return;
    } catch (const std::out_of_range&) {
    }

    try {
      const FeedSwitchEvent::FEED feed = feeds.at(source_event.event);
      const UID uid = StringToUID(source_event.fields.at("uid"));
      FeedSwitchEvent feed_switch_event;
      ExtractBaseEventFields(source_event, t, feed_switch_event);
      feed_switch_event.uid = uid;
      feed_switch_event.feed = feed;
      f(std::move(feed_switch_event));
      return;
    } catch (const std::out_of_range&) {
    }
  }

 private:
  void ExtractBaseEventFields(const MidichloriansEvent& source_event, uint64_t t, BASE_EVENT& dest_event) {
    dest_event.device_id = source_event.device_id;
    dest_event.client_id = source_event.client_id;
    dest_event.t = t;
  }
};

template <typename BASE_EVENT>
struct EventPreprocessor {
  typedef BASE_EVENT T_BASE_EVENT;

  template <typename F>
  void DispatchLogEntry(const LogEntry& log_entry, F&& f) {
    if (log_entry.m != "TICK") {
      try {
        std::unique_ptr<MidichloriansEvent> midichlorians_event;
        ParseJSON(log_entry.b, midichlorians_event);
        bricks::metaprogramming::RTTIDynamicCall<
            typename MidichloriansEventPreprocessor<BASE_EVENT>::T_SUPPORTED_TYPES>(
            midichlorians_event, midichlorians_preprocessor, log_entry.t, std::forward<F>(f));
      } catch (const bricks::ParseJSONException&) {
        std::cerr << "Unable to parse LogEvent body." << std::endl;
      }
    }
  }

 private:
  MidichloriansEventPreprocessor<BASE_EVENT> midichlorians_preprocessor;
};
