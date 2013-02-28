package in.uncod.android.bypass;

import java.util.Map;
import java.util.HashMap;

public class Element {
	
	public enum Type {
		
		// Block Element Types
		
		BLOCK_CODE(0x000),
		BLOCK_QUOTE(0x001),
		BLOCK_HTML(0x002),
		HEADER(0x003),
		HRULE(0x004),
		LIST(0x005),
		LIST_ITEM(0x006),
		PARAGRAPH(0x007),
		TABLE(0x008),
		TABLE_CELL(0x009),
		TABLE_ROW(0x00A),

		// Span Element Types

		AUTOLINK(0x10B),
		CODE_SPAN(0x10C),
		DOUBLE_EMPHASIS(0x10D),
		EMPHASIS(0x10E),
		IMAGE(0x10F),
		LINEBREAK(0x110),
		LINK(0x111),
		RAW_HTML_TAG(0x112),
		TRIPLE_EMPHASIS(0x113),
		TEXT(0x114);
		
		private final int value;

	    private Type(int value) {
	        this.value = value;
	    }
	    
	    private static final Type[] TypeValues = Type.values();
	    public static Type fromInteger(int x) {
	        return TypeValues[x];
	    }
	}
	
	String text;
	Map<String, String> attributes = new HashMap<String, String>();
	Element[] children;
	Type type;
	
	public Element(String text, int type, Element[] children) {
		this.text = text;
		this.children = children;
		this.type = Type.fromInteger(type);
	}
	
	public String getText() {
		return text;
	}
	
	public String getAttribute(String name) {
		return attributes.get(name);
	}
}