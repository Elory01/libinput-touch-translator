#ifndef TOUCHSCREENFOURFINGERSWIPEGESTURE_H
#define TOUCHSCREENFOURFINGERSWIPEGESTURE_H

#include "touch-screen/touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenFourFingerSwipeGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenFourFingerSwipeGesture(QObject *parent = nullptr);

    int finger() override {return 4;}

    GestureType type() override {return Swipe;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override {return m_is_cancelled;}

private:
    int m_current_finger_count = 0;

    bool m_is_cancelled = false;
    bool m_is_started = false;

    Direction m_last_direction = None;

    QPointF m_start_points[4];
    QPointF m_last_points[4];
    QPointF m_current_points[4];
};

#endif // TOUCHSCREENFOURFINGERSWIPEGESTURE_H
