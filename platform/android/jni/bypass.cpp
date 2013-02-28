#include "parser.h"
#include "in_uncod_android_bypass_Bypass.h"

namespace boost
{
    void throw_exception(std::exception const&) {}
}

jclass java_element_class;
jmethodID java_element_init;

jobject recurseElement(JNIEnv *env, Bypass::Element element) {
	jobjectArray elements = (jobjectArray) env->NewObjectArray(element.size(), java_element_class, 0);
	for (int i=0; i<element.size(); i++) {
		jobject jelement = recurseElement(env, element[i]);
		env->SetObjectArrayElement(elements, i, jelement);
	}
	jstring text = env->NewStringUTF(element.getText().c_str());
	jobject jelement = env->NewObject(java_element_class, java_element_init, text, element.getType(), elements);

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
	java_element_init = env->GetMethodID(java_element_class, "<init>", "(Ljava/lang/String;I[Lin/uncod/android/bypass/Element;)V");

	jobjectArray elements = (jobjectArray) env->NewObjectArray(document.size(), java_element_class, 0);

	for (int i=0; i<document.size(); i++)  {
		jobject jelement = recurseElement(env, document[i]);
		env->SetObjectArrayElement(elements, i, jelement);
	}

	jobject jdocument = env->NewObject(java_document_class, java_document_init, elements);

	return jdocument;
}

