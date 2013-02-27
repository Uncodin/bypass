#include "parser.h"
#include "in_uncod_android_bypass_Bypass.h"

JNIEXPORT jobject JNICALL Java_in_uncod_android_bypass_Bypass_processMarkdown
  (JNIEnv *env, jobject o, jstring markdown) {

	const char* str;
	str = env->GetStringUTFChars(markdown, NULL);

	Bypass::Parser parser;
	Bypass::Document document = parser.parse(str);

	env->ReleaseStringUTFChars(markdown, str);

	return NULL;
}
