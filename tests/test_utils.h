#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "gmock/gmock.h"

#include <string>
#include <chrono>
struct TestTimer {
    TestTimer();
    TestTimer(const std::string& text);
    virtual ~TestTimer();
    std::chrono::time_point<std::chrono::system_clock> Start;
    std::chrono::time_point<std::chrono::system_clock> Stop;
    std::chrono::microseconds Elapsed;
    std::string text_;
};

using namespace std;

TestTimer::TestTimer()
    : TestTimer( ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name() ) {
    text_ += string {"."} + string {::testing::UnitTest::GetInstance()->current_test_info()->name()};
}

TestTimer::TestTimer(const string& text)
    : Start { chrono::system_clock::now() }
    , text_ {text} {}

TestTimer::~TestTimer() {
    Stop = chrono::system_clock::now();
    Elapsed = chrono::duration_cast<chrono::microseconds>(Stop - Start);
    cout << endl << text_ << " elapsed time = "
        << Elapsed.count() * 0.001 << "ms" << endl;
}

class SafeQSignalSpy : public QSignalSpy {
  public:
    template <typename... Args>
    SafeQSignalSpy( Args&&... args )
        : QSignalSpy( std::forward<Args>(args)... ) {}

    bool safeWait( int timeout = 10000 ) {
        // If it has already been received
        bool result = count() > 0;
        if ( ! result ) {
            result = wait( timeout );
        }
        return result;
    }
};

#endif