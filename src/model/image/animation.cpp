#include "model/image/animation.h"

#include <utility>

#include "model/image/assets.h"

Animation::Animation(std::string classID, Skin skin)
{
    this->classID = std::move(classID);
    this->shaking = skin.shaking;
    this->status = Enums::Type::Animation::Idle;
    this->frame = 0;
}

void Animation::updateStatus(Enums::Type::Animation newStatus)
{
    this->status = newStatus;
    this->reset();
}

Magick::Image Animation::getCurrentImage() const
{
    return Assets::getInstance().skins[this->classID]["default"][(int) this->status].at(this->frame);
}

bool Animation::isFinished() const
{
    return this->frame + 1 == (int) Assets::getInstance().skins[this->classID]["default"][(int) this->status].size();
}

void Animation::nextFrame()
{
    if (this->isFinished())
    {
		if (this->status != Enums::Type::Animation::Death)
		{
			this->reset();
		}
    }
    else
    {
        this->frame++;
    }
}

void Animation::reset()
{
    this->frame = 0;
}
