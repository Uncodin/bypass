### Usage

	TextView text = (TextView) findViewById(R.id.demoText);
	Bypass bypass = new Bypass();
	String markdownString = "#Markdown!";
	CharSequence string = bypass.markdownToSpannable(markdownString);
	text.setText(string);
	text.setMovementMethod(LinkMovementMethod.getInstance());

### Maven Dependency
If you just want to include Bypass in your Maven project, add the following
dependency block to your `pom.xml`:

    <dependency>
        <groupId>in.uncod.android.bypass</groupId>
        <artifactId>bypass</artifactId>
        <type>apklib</type>
        <version>1.1</version>
    </dependency>

### Manual Build
Make sure `$ANDROID_NDK_HOME` is correctly set to the root directory of your
NDK installation. Also, if Boost lives in a non-standard directory (or you're
on Windows) you'll need to set `$BYPASS_INCLUDE_PATH` to its parent directory.

#### Maven
    cd ./platform/android
    mvn install

#### Library Project
    cd ./platform/android/library
    ndk-build

Then simply point to the library from your project.

## Speed

### Android
The following benchmarks were obtained by clocking the parsing of Sundown's readme file.

##### Bypass
    03-01 15:13:12.662: D/Bypass(8998): onCreate: begin
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      1 ms, read raw
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      2 ms, instantiated Bypass
    03-01 15:13:12.662: D/Bypass(8998): onCreate:      37 ms, convert to spannable
    03-01 15:13:12.662: D/Bypass(8998): onCreate: end, 40 ms

##### [cwac-anddown](https://github.com/commonsguy/cwac-anddown)
    03-01 15:14:44.662: D/AndDown(9047): onCreate: begin
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, read raw
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, instantiated AndDown
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      2 ms, convert to HTML
    03-01 15:14:44.662: D/AndDown(9047): onCreate:      167 ms, convert to Spanned
    03-01 15:14:44.662: D/AndDown(9047): onCreate: end, 173 ms
