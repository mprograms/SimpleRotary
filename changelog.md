# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v1.1.2]
### Fixed
- Typo in library.properties.
- Missing semicolons and Serial.begin() statement in examples\options\options.ino.

###Changed
- examples\options\options.ino to print serial output only on rotation.
- readme.md added section on troubleshooting missed pulses when using an LCD.

## [v1.1.1]
### Fixed
- Typo in readme.md example code for pushType(); Returned value should be byte not int.

## [v1.1.0]
### Added
- pushType(); function to allow for both short and long button pushes.
- Example for pushType();
### Changed
- readme.md to reflect pushType() function.

## [v1.0.0]
### Added
- Documentation to readme.md

## [v0.0.1]
### Added
- Initial creation.