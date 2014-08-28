package in.uncod.android.bypass;

import in.uncod.android.bypass.Element.Type;
import android.graphics.Typeface;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.style.BulletSpan;
import android.text.style.LeadingMarginSpan;
import android.text.style.QuoteSpan;
import android.text.style.RelativeSizeSpan;
import android.text.style.StrikethroughSpan;
import android.text.style.StyleSpan;
import android.text.style.TypefaceSpan;
import android.text.style.URLSpan;

public class Bypass {
	static {
		System.loadLibrary("bypass");
	}

	private static final float[] HEADER_SIZES = { 1.5f, 1.4f, 1.3f, 1.2f, 1.1f,
			1f, };

	public CharSequence markdownToSpannable(String markdown) {
		Document document = processMarkdown(markdown);

		CharSequence[] spans = new CharSequence[document.getElementCount()];
		for (int i = 0; i < document.getElementCount(); i++) {
			spans[i] = recurseElement(document.getElement(i));
		}

		return TextUtils.concat(spans);
	}

	private native Document processMarkdown(String markdown);

	private CharSequence recurseElement(Element element) {

		CharSequence[] spans = new CharSequence[element.size()];
		for (int i = 0; i < element.size(); i++) {
			spans[i] = recurseElement(element.children[i]);
		}

		CharSequence concat = TextUtils.concat(spans);

		SpannableStringBuilder builder = new SpannableStringBuilder();
		String text = element.getText();
		if (element.size() == 0
				&& element.getParent() != null
                && element.getParent().getType() != Type.BLOCK_CODE) {
			text = text.replace('\n', ' ');
		}
		switch (element.getType()) {
			case LIST:
				if (element.getParent() != null
					&& element.getParent().getType() == Type.LIST_ITEM) {
					builder.append("\n");
				}
				break;
			case LINEBREAK:
				builder.append("\n");
				break;
			case LIST_ITEM:
				builder.append("\u2022");
				break;
		}
		builder.append(text);
		builder.append(concat);
		if (element.getType() == Type.LIST && element.getParent() != null) {

		} else if (element.getType() == Type.LIST_ITEM) {
			if (element.size() > 0 && element.children[element.size()-1].isBlockElement()) {
				
			}
			else {
				builder.append("\n");
			}
		} else if (element.isBlockElement()) {
			builder.append("\n\n");
		}

		if (element.getType() == Type.HEADER) {
			String levelStr = element.getAttribute("level");
			int level = Integer.parseInt(levelStr);
			setSpan(builder, new RelativeSizeSpan(HEADER_SIZES[level]));
			setSpan(builder, new StyleSpan(Typeface.BOLD));
		} else if (element.getType() == Type.LIST_ITEM
				&& element.getParent().getParent() != null) {
			setSpan(builder, new LeadingMarginSpan.Standard(20));
		} else if (element.getType() == Type.EMPHASIS) {
			setSpan(builder, new StyleSpan(Typeface.ITALIC));
		} else if (element.getType() == Type.DOUBLE_EMPHASIS) {
			setSpan(builder, new StyleSpan(Typeface.BOLD));
		} else if (element.getType() == Type.TRIPLE_EMPHASIS) {
			setSpan(builder, new StyleSpan(Typeface.BOLD_ITALIC));
		} else if (element.getType() == Type.CODE_SPAN) {
			setSpan(builder, new TypefaceSpan("monospace"));
		} else if (element.getType() == Type.LINK) {
			setSpan(builder, new URLSpan(element.getAttribute("link")));
		} else if (element.getType() == Type.BLOCK_QUOTE) {
			setSpan(builder, new QuoteSpan());
			setSpan(builder, new StyleSpan(Typeface.ITALIC));
		} else if (element.getType() == Type.STRIKETHROUGH) {
			setSpan(builder, new StrikethroughSpan());
		}

		return builder;
	}

	private static void setSpan(SpannableStringBuilder builder, Object what) {
		builder.setSpan(what, 0, builder.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
	}
}
