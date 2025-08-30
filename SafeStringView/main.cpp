#include "safestringview.h"

#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <cassert>


static void test_basic() {
const char* lit = "Hello World"; // литерал → StaticSafe
safe_string_view v1(lit);


assert(v1.fsize() == 11);
assert(v1.front() == 'H');
assert(v1.back() == 'd');
auto sub = v1.substr(6, 5);
assert(sub.to_string() == std::string("World"));
assert(v1.find("World") == 6);
assert(v1.is_valid());
assert(v1.is_string_literal());
}


static void test_dangling_prevention() {
safe_string_view view;
{
safe_string s("Temporary string");
view = s.view();
assert(view.is_valid());
assert(view.to_string() == "Temporary string");
} // s уничтожен → view должен стать невалидным


assert(!view.is_valid());
bool thrown = false;
try {
(void)view[0];
} catch (const std::runtime_error&) {
thrown = true;
}
assert(thrown && "Ожидали исключение при доступе к невалидному view");
}


static void test_thread_safety() {
safe_string s("Thread safety test");
safe_string_view v = s.view();


std::vector<std::thread> readers;
std::atomic<bool> keep{true};


for (int i = 0; i < 4; ++i) {
readers.emplace_back([&]() {
while (keep.load()) {
if (v.is_valid() && !v.empty()) {
volatile char c = v[0];
(void)c;
}
std::this_thread::yield();
}
});
}


std::this_thread::sleep_for(std::chrono::milliseconds(50));
keep = false;
for (auto& t : readers) t.join();
}


int main() {
test_basic();
test_dangling_prevention();
test_thread_safety();
std::cout << "All tests passed\n";
}