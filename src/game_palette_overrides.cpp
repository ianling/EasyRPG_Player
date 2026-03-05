#include "game_palette_overrides.h"

#include "output.h"

void Game_PaletteOverrides::AddOverride(const int id, const OverrideParams &params) {
    overridesById[id] = params;
    changeSeqNumber += 1;
}

void Game_PaletteOverrides::Erase(const int id) {
    overridesById.erase(id);
    changeSeqNumber += 1;
}

void Game_PaletteOverrides::EraseAll() {
    overridesById.clear();
    changeSeqNumber += 1;
}

void Game_PaletteOverrides::Update() {}

bool Game_PaletteOverrides::OverridesChanged(const uint32_t lastKnownChangeSeqNum) const {
    return lastKnownChangeSeqNum != changeSeqNumber;
}

uint32_t Game_PaletteOverrides::CurrentChangeSeqNumber() const {
    return changeSeqNumber;
}

void Game_PaletteOverrides::OnMapChange() {
    std::vector<int> idsToErase;
    for (const auto& [id, override] : overridesById) {
        if ((override.flags & EraseOnMapChange) != 0)
            idsToErase.push_back(id);
    }

    for (const auto& id : idsToErase) {
        overridesById.erase(id);
    }

    Output::Debug("removing {} palette overrides on map change", idsToErase.size());
    changeSeqNumber += 1;
}

Game_PaletteOverrides::OverrideParams &Game_PaletteOverrides::GetOverride(const int id) {
    return overridesById[id];
}

Game_PaletteOverrides::OverrideParams* Game_PaletteOverrides::GetOverridePtr(const int id) {
    return &overridesById[id];
}

std::vector<const Game_PaletteOverrides::OverrideParams*> Game_PaletteOverrides::GetEffectiveOverrides(const int affects_flag, const int target_int, const std::string &target_str) {
    std::vector<const OverrideParams*> result;

    for (auto& [_, override] : overridesById) {
        if ((override.affects_flags & affects_flag) != 0
            && (override.target_int == -1 || override.target_int == target_int)
            && (override.target_str.empty() || override.target_str == target_str))
            result.push_back(&override);
    }
    // Output::Debug("got {} overrides for {} {}", result.size(), target_int, target_str);

    return result;
}
