package in.uncod.android.bypass;

import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.TextUtils;

public class Bypass {
	static {
		System.loadLibrary("spandown");
	}

	public CharSequence markdownToSpannable(String markdown) {
		Document document = processMarkdown(markdown);
		
		Spannable[] spans = new Spannable[document.getElementCount()];
		for (int i=0; i<document.getElementCount(); i++) {
			spans[i] = elementToSpannable(document.getElement(i));
		}
		
		return TextUtils.concat(spans);
	}
	
	private native Document processMarkdown(String markdown);
	
	private Spannable elementToSpannable(Element element) {
		SpannableStringBuilder stringBuilder = new SpannableStringBuilder();
		
		//MAGIC!!
		
		return stringBuilder;
	}
}
