#include "esphome.h"

struct Event {
  std::string title;
  esphome::time::ESPTime start_time;
  esphome::time::ESPTime end_time;
};

std::vector<Event> events;

static const uint16_t LEFT_BORDER = 10;
static const uint16_t LEFT_TITLE = 110;

static const uint16_t WIDTH = 600;
static const uint16_t HEIGHT = 800;

static const uint32_t ONE_DAY = 60 * 60 * 24;

void add_event(const std::string title, uint32_t start_time, uint32_t end_time) {
  Event event{
    .title = title,
    .start_time = esphome::time::ESPTime::from_epoch_local(start_time),
    .end_time = esphome::time::ESPTime::from_epoch_local(end_time)
  };
  events.push_back(event);
}

void clear_events() {
  events.clear();
}

void draw_agenda(esphome::display::DisplayBuffer &it) {

  it.fill(COLOR_ON);

  it.print(LEFT_BORDER, 5, &id(opensans_24), COLOR_OFF, TextAlign::TOP_LEFT, "AGENDA");
  it.strftime(WIDTH - LEFT_BORDER, 5, &id(opensans_24), COLOR_OFF, TextAlign::TOP_RIGHT, "%A %d %B %Y", id(esptime).now());
  it.filled_rectangle(LEFT_BORDER, 35, WIDTH - (2 * LEFT_BORDER), 2, COLOR_OFF);

  uint16_t height = 50;
  uint8_t day = id(esptime).now().day_of_month;

  for (Event event : events) {
    // DAY CHANGED - DRAW DAY HEADER
    if (event.start_time.day_of_month != day) {
      if (height != 50) height += 10;
      it.strftime(LEFT_BORDER, height, &id(visitor_20), COLOR_OFF, TextAlign::TOP_LEFT, "%A %d %B %Y", event.start_time);
      it.line(LEFT_BORDER, height + 22, LEFT_BORDER + 200, height + 22);

      height += 23;
      day = event.start_time.day_of_month;
    }
    // END DAY HEADER

    if (event.end_time.timestamp - event.start_time.timestamp == ONE_DAY) {
      it.print(LEFT_TITLE - 10, height + 5, &id(visitor_20), COLOR_OFF, TextAlign::TOP_RIGHT, "ALL");
      it.print(LEFT_TITLE - 10, height + 22, &id(visitor_20), COLOR_OFF, TextAlign::TOP_RIGHT, "DAY");
    } else {
      std::string start = event.start_time.strftime("%I:%M%p");
      std::string end = event.end_time.strftime("%I:%M%p");

      if (start.substr(0,1) == "0")
        start = start.substr(1);
      if (end.substr(0,1) == "0")
        end = end.substr(1);

      it.print(LEFT_TITLE - 10, height + 5, &id(visitor_20), COLOR_OFF, TextAlign::TOP_RIGHT, start.c_str());
      it.print(LEFT_TITLE - 10, height + 22, &id(visitor_20), COLOR_OFF, TextAlign::TOP_RIGHT, end.c_str());
    }

    it.print(LEFT_TITLE, height, &id(inter_36), COLOR_OFF, TextAlign::TOP_LEFT, event.title.c_str());

    height += 40;

    if (height >= 750 - 73) break;
  }

  it.filled_rectangle(LEFT_BORDER, 750, WIDTH - (2 * LEFT_BORDER), 2, COLOR_OFF);
  it.printf(LEFT_BORDER, 795, &id(opensans_24), COLOR_OFF, TextAlign::BOTTOM_LEFT, "Battery: %.2fV (%.0f%%)", id(battery_voltage).state, id(battery_level).state);
  it.strftime(WIDTH - LEFT_BORDER, 795, &id(opensans_24), COLOR_OFF, TextAlign::BOTTOM_RIGHT, "Last Updated: %I:%M%p", id(esptime).now());

}
