#include "parser.h"
#include "in_uncod_android_bypass_Bypass.h"

namespace boost
{
    void throw_exception(std::exception const&) {}
}

jobject recurseElement(Bypass::Element element) {

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
	jclass java_element_class = env->FindClass("in/uncod/android/bypass/Element");
	jmethodID java_element_init = env->GetMethodID(java_element_class, "<init>", "(Ljava/lang/String;I[Lin/uncod/android/bypass/Element;)V");

	jobjectArray elements = (jobjectArray) env->NewObjectArray(document.size(), java_element_class, 0);

	for (int i=0; i<document.size(); i++)  {
		jobject jelement = recurseElement(document[i]);
		env->SetObjectArrayElement(elements, i, jelement);
	}

	jobject jdocument = env->NewObject(java_document_class, java_document_init, elements);

	return jdocument;
}

