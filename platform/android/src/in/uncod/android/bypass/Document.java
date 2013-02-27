package in.uncod.android.bypass;

public class Document {

	List<Element> elements;
	
	public Document(List<Element> elements) {
		this.elements = elements;
	}
	
	public List<Element> getElements() {
		return elements;
	}
	
	public int getElementCount() {
		return elements.size();
	}
	
	public Element getElement(int pos) {
		return elements.get(pos);
	}
}
