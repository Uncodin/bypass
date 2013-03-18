# Bypass iOS

Bypass for iOS is a markdown rendering library that populates
`NSAttributedString` with attributes that are appropriate for display in
a `UITextView` or in a `BPMarkdownView`.

Because `UITextView`s represent
`NSAttributedString`s internally as HTML, they render much slower than directly
rendering with Core Text. `BPMarkdownView` addresses this issue and provides an
alternative for when you have a lot of read-only text.

## Requirements

Bypass requires iOS version 6.0 or greater.

## Setup

* Clone bypass from Github
* Drag the Bypass.xcodeproj from Finder into your project
* Open the target settings
* Select the Build Phases tab
* Add Bypass as a target dependency
* Add libBypass.a to the list of libraries to link against

## Performance

Visit the [profiling data](./Profiling/index.md) page to see information on
speed and performance.
