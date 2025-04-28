#pragma once

#include <Magick++.h>
#include <numeric>
#include <dpp/nlohmann/json.hpp>

#include "utility/utils.h"
#include "model/image/assets.h"
#include "model/image/animation.h"
#include "model/fight/position.h"
#include "model/fight/participant.h"
#include "model/entity/skill/skillAnimation.h"

struct Entity;

using json = nlohmann::json;

struct Text
{
	std::string text;
	Position position{Position(0, 0)};
	double fontSize { 12 };
	bool xCentered{true};
	bool yCentered{true};
	Magick::Color color{Magick::Color("white")};
	bool shadow{true};
};

enum class Transition {
	FADE_IN,
	FADE_OUT,
	MOVE
};

struct Gif
{
	/**
	 * Initialize the gif with the given width and height and add a new image to the gif.
	 * @param width {int} The width of the gif.
	 * @param height {int} The height of the gif.
	 */
	Gif(int width, int height);
	/**
	 * Initialize the gif with a default width and height and add a new image to the gif.
	 */
	Gif();
	/**
	 * Destroy the gif.
	 */
	~Gif();

	int width;
	int height;
	std::vector<Magick::Image> frames = {};
	Magick::Image currentImage;

private:
	static Position getAttackBarPosition(int index, int tokenWidth);

public:

	/**
	 * Add current image to the gif
	 */
	void addImage();
	/**
	* Initialize a new image
	*/
	void initializeNewImage();
	/**
	 * Add current image to the gif and initialize a new image
	 */
	void addAndInitializeNewImage();
	/**
	 * Generate a gif from frames
	 * @returns string decoded base64 encoded gif
	 */
	std::string generate();
	/**
	 * Generate an image from the last frame
	 * @return string decoded base64 encoded image
	 */
	std::string generateLastFrame();

	/**
	 * Draw text to current image
	 * @param text {Text} The text to draw
	 */
	void drawText(const Text& text);
	/**
	 * Draw an image centered on the current image
	 */
	void drawImage(const Magick::Image& image, Position position, bool xCentered = true, bool yCentered = true);
	/**
	 * Draw a part of an image centered on the current image
	 */
	void drawImagePart(Magick::Image image, Position position, double widthPercent = 1.0, int xOffset = 0);
	/**
	 * Draw a health bar to current image
	 * @param percent {double} Percent of health (0.0 - 1.0)
	 * @param position {Position} Position of health bar
	 */
	void drawHpBar(const Participant& participant, Position position);
	/**
	 * Draw all participant on the global attack bar
	 * @param participants {Participants} Participants to draw
	 */
	void drawAttackBar(Participants participants);
	/**
	* Draw an entity to current image
	*/
	void drawEntity(const Participant& participant);
	/**
	* Draw an entity to current image
	* @param participant {Participant} Participant to draw
	 * @param displayNumber {bool} True if the number of the participant should be displayed
	*/
	void drawParticipant(const Participant& participant, bool displayNumber);
	/**
	* Replace all color in the image by the given color by adapting the value.
	* @param image {Magick::Image} Image to modify
	* @param replaceColor {Magick::Color} Color to replace
	*/
	static void replaceColorSDF(Magick::Image& image, const Magick::Color& replaceColor);
	/**
	 * Draw a line to current image
	 * @param start {Position} Start position of line
	 * @param end {Position} End position of line
	 * @param width {int} Width of line
	 * @param color {Magick::Color} Color of line
	 */
	void drawLine(Position start, Position end, int lineWidth, const Magick::Color& color);
	/**
	 * Draw current attacker on his active turn with some information
	 * @param attacker {Participant} Attacker to draw
	 */
	void drawAttacker(const Participant& attacker);

	void drawSkillAnimation(SkillAnimationController animationController);
};
