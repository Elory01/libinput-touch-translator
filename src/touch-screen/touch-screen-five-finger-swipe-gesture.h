/*
 * Libinput Touch Translator
 *
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

#ifndef TOUCHSCREENFIVEFINGERSWIPEGESTURE_H
#define TOUCHSCREENFIVEFINGERSWIPEGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenFiveFingerSwipeGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenFiveFingerSwipeGesture(QObject *parent = nullptr);

    int finger() override {return 5;}

    GestureType type() override {return Swipe;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override {return m_isCancelled;}

private:
    int m_currentFingerCount = 0;

    bool m_isCancelled = false;
    bool m_isStarted = false;

    Direction m_lastDirection = None;

    QPointF m_startPoints[5];
    QPointF m_lastPoints[5];
    QPointF m_currentPoints[5];
};

#endif // TOUCHSCREENFIVEFINGERSWIPEGESTURE_H
