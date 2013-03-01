package in.uncod.andrpod.bypass.bypass_test.test;

import in.uncod.android.bypass.Bypass;
import android.test.InstrumentationTestCase;
import android.text.Spannable;
import android.text.style.RelativeSizeSpan;
import android.text.style.StyleSpan;

public class BasicTests extends InstrumentationTestCase {

	Bypass bypass;

	@Override
	protected void setUp() throws Exception {
		super.setUp();

		bypass = new Bypass();
	}

	@Override
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	public void testEmptyString() {
		bypass.markdownToSpannable("");
	}

	public void testHeaderString() {
		Spannable spannable = (Spannable) bypass.markdownToSpannable("Header!!\n--------");
		StyleSpan[] spans = spannable.getSpans(0, spannable.length(), StyleSpan.class);
		assertEquals(1, spans.length);
		RelativeSizeSpan[] sizeSpan = spannable.getSpans(0, spannable.length(), RelativeSizeSpan.class);
		assertEquals(1, sizeSpan.length);
		assertEquals('\n', spannable.charAt(spannable.length() - 1));
	}
}
