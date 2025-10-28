//
// Created by Loboda Alexey on 21.05.2020.
//

#include <string>
#include <jni.h>
#include "codec/CodecOpus.h"
#include "utils/SamplesConverter.h"

CodecOpus* codec = nullptr;

//
// Creation and Destruction
//

extern "C"
JNIEXPORT jlong JNICALL
Java_com_theeasiestway_opus_Opus_createCodec(JNIEnv *env, jobject thiz) {
    CodecOpus* newCodec = new CodecOpus();
    return reinterpret_cast<jlong>(newCodec);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_opus_Opus_destroyCodec(JNIEnv *env, jobject thiz, jlong handle) {
    if (handle != 0) {
        CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
        delete codecInstance;
    }
}

//
// Encoding
//

extern "C"
JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Opus_encoderInit(JNIEnv *env, jobject thiz, jlong handle, jint sample_rate, jint num_channels, jint application) {
    if (handle == 0) return -1;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    return codecInstance->encoderInit(sample_rate, num_channels, application);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_theeasiestway_opus_Opus_encoderSetBitrate(JNIEnv *env, jobject thiz, jlong handle, jint bitrate) {
    if (handle == 0) return -1;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    return codecInstance->encoderSetBitrate(bitrate);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_theeasiestway_opus_Opus_encoderSetComplexity(JNIEnv *env, jobject thiz, jlong handle, jint complexity) {
    if (handle == 0) return -1;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    return codecInstance->encoderSetComplexity(complexity);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_opus_Opus_encode__J_3BI(JNIEnv *env, jobject thiz, jlong handle, jbyteArray bytes, jint frame_size) {
    if (handle == 0) return nullptr;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    
    jbyte *nativeBytes = env->GetByteArrayElements(bytes, 0);
    std::vector<uint8_t> encodedData = codecInstance->encode((uint8_t *) nativeBytes, frame_size);
    int encodedSize = encodedData.size();
    if (encodedSize <= 0) return nullptr;

    jbyteArray result = env->NewByteArray(encodedSize);
    env->SetByteArrayRegion(result, 0, encodedSize, (jbyte *) encodedData.data());
    env->ReleaseByteArrayElements(bytes, nativeBytes, 0);

    return result;
}

extern "C"
JNIEXPORT jshortArray JNICALL
Java_com_theeasiestway_opus_Opus_encode__J_3SI(JNIEnv *env, jobject thiz, jlong handle, jshortArray shorts, jint frame_size) {
    if (handle == 0) return nullptr;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    
    jshort *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<short> encodedData = codecInstance->encode(nativeShorts, length, frame_size);
    int encodedSize = encodedData.size();
    if (encodedSize <= 0) return nullptr;

    jshortArray result = env->NewShortArray(encodedSize);
    env->SetShortArrayRegion(result, 0, encodedSize, encodedData.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0);

    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_opus_Opus_encoderRelease(JNIEnv *env, jobject thiz, jlong handle) {
    if (handle == 0) return;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    codecInstance->encoderRelease();
}

//
// Decoding
//

extern "C"
JNIEXPORT jint JNICALL
Java_com_theeasiestway_opus_Opus_decoderInit(JNIEnv *env, jobject thiz, jlong handle, jint sample_rate, jint num_channels) {
    if (handle == 0) return -1;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    return codecInstance->decoderInit(sample_rate, num_channels);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_opus_Opus_decode__J_3BII(JNIEnv *env, jobject thiz, jlong handle, jbyteArray bytes, jint frame_size, jint fec) {
    if (handle == 0) return nullptr;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    
    jbyte *nativeBytes = env->GetByteArrayElements(bytes, 0);
    jint length = env->GetArrayLength(bytes);

    std::vector<uint8_t> encodedData = codecInstance->decode((uint8_t *) nativeBytes, length, frame_size, fec);
    int encodedSize = encodedData.size();
    if (encodedSize <= 0) return nullptr;

    jbyteArray result = env->NewByteArray(encodedSize);
    env->SetByteArrayRegion(result, 0, encodedSize, (jbyte *) encodedData.data());
    env->ReleaseByteArrayElements(bytes, nativeBytes, 0);

    return result;
}

extern "C"
JNIEXPORT jshortArray JNICALL
Java_com_theeasiestway_opus_Opus_decode__J_3SII(JNIEnv *env, jobject thiz, jlong handle, jshortArray shorts, jint frame_size, jint fec) {
    if (handle == 0) return nullptr;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    
    jshort *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<short> decodedData = codecInstance->decode(nativeShorts, length, frame_size, fec);
    int decodedSize = decodedData.size();
    if (decodedSize <= 0) return nullptr;

    jshortArray result = env->NewShortArray(decodedSize);
    env->SetShortArrayRegion(result, 0, decodedSize, decodedData.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0);

    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_opus_Opus_decoderRelease(JNIEnv *env, jobject thiz, jlong handle) {
    if (handle == 0) return;
    CodecOpus* codecInstance = reinterpret_cast<CodecOpus*>(handle);
    codecInstance->decoderRelease();
}

//
// Utils
//

extern "C"
JNIEXPORT jshortArray JNICALL
Java_com_theeasiestway_opus_Opus_convert___3B(JNIEnv *env, jobject thiz, jbyteArray bytes) {
    uint8_t *nativeBytes = (uint8_t *) env->GetByteArrayElements(bytes, 0);
    jint length = env->GetArrayLength(bytes);

    std::vector<short> shorts = SamplesConverter::convert(&nativeBytes, length);
    int size = shorts.size();
    if (!size) return nullptr;

    jshortArray result = env->NewShortArray(size);
    env->SetShortArrayRegion(result, 0, size, shorts.data());
    env->ReleaseByteArrayElements(bytes, (jbyte *) nativeBytes, 0);

    return result;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_opus_Opus_convert___3S(JNIEnv *env, jobject thiz, jshortArray shorts) {
    short *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<uint8_t> bytes = SamplesConverter::convert(&nativeShorts, length);
    int size = bytes.size();
    if (!size) return nullptr;

    jbyteArray result = env->NewByteArray(size);
    env->SetByteArrayRegion(result, 0, size, (jbyte *) bytes.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0);

    return result;
}