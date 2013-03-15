# Bypass iOS

Bypass for iOS is a markdown rendering library that populates
`NSAttributedString` with attributes that are appropriate for display in
a `UITextView` or in a `BPMarkdownView`.

Because `UITextView`s represent
`NSAttributedString`s internally as HTML, they render much slower than directly
rendering with Core Text. `BPMarkdownView` solves this problem by dropping down
to Core Text and rendering text directly.

## Requirements

Bypass requires iOS version 6.0 or greater.

## Inclusion

* Clone bypass from Github
* Drag the Bypass.xcodeproj from Finder into your project
* Open the target settings
* Select the Build Phases tab
* Add Bypass as a target dependency
* Add libBypass.a to the list of libraries to link against


[Profiling data](./Profiling/index.md)
