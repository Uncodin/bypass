bypass
======

Skip the HTML, Bypass takes markdown renders it on Android and iOS

Needs Boost!

Speed
=====
Android
-------
###Parsing the Readme for Sundown:

#### Bypass
    03-01 15:13:12.662: D/Bypass(8998): onCreate: begin
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      1 ms, read raw
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      2 ms, instantiated Bypass
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      37 ms, convert to spannable
    03-01 15:13:12.662: D/Bypass(8998): onCreate: end, 40 ms

#### [cwac-anddown](https://github.com/commonsguy/cwac-anddown)
    03-01 15:14:44.662: D/AndDown(9047): onCreate: begin
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, read raw
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, instantiated AndDown
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, convert to HTML
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      167 ms, convert to Spanned
    03-01 15:14:44.662: D/AndDown(9047): onCreate: end, 173 ms

