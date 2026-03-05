/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_GAME_PALETTE_OVERRIDES_H
#define EP_GAME_PALETTE_OVERRIDES_H

#include <string>
#include <deque>
#include <map>

#include "async_handler.h"

// Game_PaletteOverrides wraps a backing store of palette overrides,
// providing convenience methods for adding/removing/modifying/retrieving palette overrides.
class Game_PaletteOverrides {
public:
	Game_PaletteOverrides() = default;

	enum Flags {
		EraseOnMapChange = 1,
		IgnoreAlpha = 32,
	};

	enum Affects {
		Chipset = 1,
		Charset = 2,
		Picture = 4,
		Panorama = 8,
	};

	// OverrideParams provides the parameters available for the CommandEasyRpgOverrideColor event command.
	// A Palette Override defines an RGBA color that should be replaced with another RGBA color.
	// The affects_flags and target fields can be used to select what is affected.
	struct OverrideParams {
		uint8_t red_original = 255;
		uint8_t green_original = 255;
		uint8_t blue_original = 255;
		uint8_t alpha_original = 255;
		uint8_t red_replacement = 255;
		uint8_t green_replacement = 255;
		uint8_t blue_replacement = 255;
		uint8_t alpha_replacement = 255;
		int flags = EraseOnMapChange | IgnoreAlpha;
		// See Affects
		int affects_flags = Chipset | Charset | Picture | Panorama;
		// can be used to limit the palette override to a particular integer ID of another object type (corresponding to affects_flags)
		// ignored if -1.
		int target_int = -1;
		// can be used to limit the palette override to a particular string ID of another object type (corresponding to affects_flags).
		// for pictures, this is the image filename.
		// ignored if empty.
		std::string target_str{};
	};

	void AddOverride(int id, const OverrideParams& params);
	void Erase(int id);
	void EraseAll();

	// Update is called once per frame.
	void Update();

	void OnMapChange();

	// OverridesChanged returns true if the palette overrides have been modified since the given change seq number
	[[nodiscard]] bool OverridesChanged(uint32_t lastKnownChangeSeqNum) const;

	[[nodiscard]] uint32_t CurrentChangeSeqNumber() const;

	OverrideParams& GetOverride(int id);
	OverrideParams* GetOverridePtr(int id);
	// GetEffectiveOverrides returns overrides affecting the given flag (see Affects) and ID.
	// Callers are intended to self-describe in the method params in order to retrieve overrides that affect themselves.
	// The given affects_flag value is bitwise &'d with the same field on each palette override.
	// The id is compared to each overrides' target_int and/or target_str field, depending on which type is given
	std::vector<const OverrideParams*> GetEffectiveOverrides(int affects_flag, int target_int, const std::string &target_str);

private:
	std::map<int, OverrideParams> overridesById;
	uint32_t changeSeqNumber = 1;
};

#endif
