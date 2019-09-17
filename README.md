# NWN-Helper

Cross-platform program for editing NWN-EE files outside of toolset. NWN-Helper is meant
to make WB's life much easier by doing. So far It's mostly work in progress.

Ever wanted added/modify spells/feats/skills and got discouraged by the amount of details
you need to go through to do it? Then NWN-Helper is meant for you. The goal is to make the
process as easy as possible. So far it should work with NWN 1.69, but I will be following
NWN-EE updates as they come out.

## Requirements

- `C++17 compatible compiler`
- `wxWidgets 3.1.2+`
- `CMake 3.0.2+`

## Future plans

- Feat/Skill and other 2da file support
- Filter for dataview
- 2da/tlk merge into current project (automatic + manual review)
- Create hakpaks from project files to further ease importing settings to NWN
- Add TOML support to include new NWN-EE dev patches
- (to be further expanded as development goes)

## Acknowledgments

- [Liareth](https://github.com/Liareth) for his [NWNFileFormats](https://github.com/Liareth/NWNFileFormats) without
which I would have probably given up on this program (as it would be annoying to code)
- [SimpleIni](https://github.com/brofield/simpleini/tree/fe082fa81f4a55ddceb55056622136be616b3c6f) used as submodule
- BeamDog for reviving my childhood
