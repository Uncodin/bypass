#include "parser.h"
#include "in_uncod_android_bypass_Bypass.h"

namespace boost
{
    void throw_exception(std::exception const&) {}
}

jclass java_element_class;
jmethodID java_element_init;
jmethodID java_element_addAttr;
jmethodID java_element_setChildren;
jmethodID java_element_setParent;

jobject recurseElement(JNIEnv *env, Bypass::Element element, jobject parent) {
	
	jstring text = env->NewStringUTF(element.getText().c_str());
	jobject jelement = env->NewObject(java_element_class, java_element_init, text, element.getType());
	env->DeleteLocalRef(text);

	if (parent != NULL) {
		env->CallVoidMethod(jelement, java_element_setParent, parent);
	}

	if (element.size() > 0) {
		jobjectArray elements = (jobjectArray) env->NewObjectArray(element.size(), java_element_class, 0);
		for (int i=0; i<element.size(); i++) {
			jobject childElement = recurseElement(env, element[i], jelement);
			env->SetObjectArrayElement(elements, i, childElement);
			env->DeleteLocalRef(childElement);
		}

		env->CallVoidMethod(jelement, java_element_setChildren, elements);
	}

	Bypass::Element::AttributeMap::iterator it = element.attrBegin();
	for (; it != element.attrEnd(); ++it) {
		jstring name = env->NewStringUTF(it->first.c_str());
		jstring value = env->NewStringUTF(it->second.c_str());

		env->CallVoidMethod(jelement, java_element_addAttr, name, value);

		env->DeleteLocalRef(name);
		env->DeleteLocalRef(value);
	}

	return jelement;
}

JNIEXPORT jobject JNICALL Java_in_uncod_android_bypass_Bypass_processMarkdown
  (JNIEnv *env, jobject o, jstring markdown) {

	const char* str;
	str = env->GetStringUTFChars(markdown, NULL);

	Bypass::Parser parser;
	Bypass::Document document = parser.parse(str);

	env->ReleaseStringUTFChars(markdown, str);

	jclass java_document_class = env->FindClass("in/uncod/android/bypass/Document");
	jmethodID java_document_init = env->GetMethodID(java_document_class, "<init>", "([Lin/uncod/android/bypass/Element;)V");
	java_element_class = env->FindClass("in/uncod/android/bypass/Element");
	java_element_init = env->GetMethodID(java_element_class, "<init>", "(Ljava/lang/String;I)V");
	java_element_setChildren = env->GetMethodID(java_element_class, "setChildren", "([Lin/uncod/android/bypass/Element;)V");
	java_element_setParent = env->GetMethodID(java_element_class, "setParent", "(Lin/uncod/android/bypass/Element;)V");
	java_element_addAttr = env->GetMethodID(java_element_class, "addAttribute", "(Ljava/lang/String;Ljava/lang/String;)V");

	jobjectArray elements = (jobjectArray) env->NewObjectArray(document.size(), java_element_class, 0);

	for (int i=0; i<document.size(); i++)  {
		jobject jelement = recurseElement(env, document[i], NULL);
		env->SetObjectArrayElement(elements, i, jelement);
		env->DeleteLocalRef(jelement);
	}

	jobject jdocument = env->NewObject(java_document_class, java_document_init, elements);

	return jdocument;
}

