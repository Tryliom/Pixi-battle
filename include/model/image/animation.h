#pragma once

#include <Magick++.h>

#include "utility/enums.h"
#include "model/entity/classType/skin.h"

struct Animation final
{
    Animation(std::string classID, Skin skin);

    std::string classID;
    Enums::Type::Animation status;
    bool shaking;
    int frame;

    void updateStatus(Enums::Type::Animation newStatus);

    [[nodiscard]] Magick::Image getCurrentImage() const;
    [[nodiscard]] bool isFinished() const;
	void nextFrame();
    void reset();
};
