package com.theeasiestway.opus

import android.util.Log


//
// Created by Loboda Alexey on 21.05.2020.
//

class Opus {

    companion object {
        val TAG = "CodecOpus"

        init {
            try { System.loadLibrary("easyopus") }
            catch (e: Exception) { Log.e(TAG, "Couldn't load opus library: $e") }
        }
    }

    private var nativeHandle: Long = 0

    init {
        nativeHandle = createCodec()
    }

    fun release() {
        if (nativeHandle != 0L) {
            destroyCodec(nativeHandle)
            nativeHandle = 0
        }
    }

    protected fun finalize() {
        release()
    }

    private external fun createCodec(): Long
    private external fun destroyCodec(handle: Long)

    //
    // Encoder
    //

    fun encoderInit(sampleRate: Constants.SampleRate, channels: Constants.Channels, application: Constants.Application): Int {
        return encoderInit(nativeHandle, sampleRate.v, channels.v, application.v)
    }
    private external fun encoderInit(handle: Long, sampleRate: Int, numChannels: Int, application: Int): Int

    fun encoderSetBitrate(bitrate: Constants.Bitrate): Int {
        return encoderSetBitrate(nativeHandle, bitrate.v)
    }
    private external fun encoderSetBitrate(handle: Long, bitrate: Int): Int

    fun encoderSetComplexity(complexity: Constants.Complexity): Int {
        return encoderSetComplexity(nativeHandle, complexity.v)
    }
    private external fun encoderSetComplexity(handle: Long, complexity: Int): Int

    fun encode(bytes: ByteArray, frameSize: Constants.FrameSize): ByteArray? {
        return encode(nativeHandle, bytes, frameSize.v)
    }
    private external fun encode(handle: Long, bytes: ByteArray, frameSize: Int): ByteArray?

    fun encode(shorts: ShortArray, frameSize: Constants.FrameSize): ShortArray? {
        return encode(nativeHandle, shorts, frameSize.v)
    }
    private external fun encode(handle: Long, shorts: ShortArray, frameSize: Int): ShortArray?
    
    fun encoderRelease() {
        encoderRelease(nativeHandle)
    }
    private external fun encoderRelease(handle: Long)

    //
    // Decoder
    //

    fun decoderInit(sampleRate: Constants.SampleRate, channels: Constants.Channels): Int {
        return decoderInit(nativeHandle, sampleRate.v, channels.v)
    }
    private external fun decoderInit(handle: Long, sampleRate: Int, numChannels: Int): Int

    fun decode(bytes: ByteArray, frameSize: Constants.FrameSize, fec: Int = 0): ByteArray? {
        return decode(nativeHandle, bytes, frameSize.v, fec)
    }
    private external fun decode(handle: Long, bytes: ByteArray, frameSize: Int, fec: Int): ByteArray?

    fun decode(shorts: ShortArray, frameSize: Constants.FrameSize, fec: Int = 0): ShortArray? {
        return decode(nativeHandle, shorts, frameSize.v, fec)
    }
    private external fun decode(handle: Long, shorts: ShortArray, frameSize: Int, fec: Int): ShortArray?
    
    fun decoderRelease() {
        decoderRelease(nativeHandle)
    }
    private external fun decoderRelease(handle: Long)

    //
    // Utils
    //

    external fun convert(bytes: ByteArray): ShortArray?
    external fun convert(shorts: ShortArray): ByteArray?
}
