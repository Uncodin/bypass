package in.uncod.android.bypass;

public class Bypass {
	static {
		System.loadLibrary("spandown");
	}

	private native Document processMarkdown(String markdown);
}
