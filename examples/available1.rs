use env_logger::{init_from_env, Env, DEFAULT_FILTER_ENV};
use hwcodec::{
    common::{Quality::*, RateControl::*},
    ffmpeg::AVPixelFormat,
    ffmpeg_ram::{
        decode::Decoder,
        encode::{EncodeContext, Encoder},
        CodecInfo,
    },
};

#[cfg(feature = "vram")]
use hwcodec::{
    common::MAX_GOP,
    vram::{
        decode, encode, DecodeContext, DynamicContext, EncodeContext, FeatureContext,
    }
};
use std::time::Instant;

#[cfg(not(feature = "vram"))]
fn main() {
    init_from_env(Env::default().filter_or(DEFAULT_FILTER_ENV, "info"));

    let ctx = EncodeContext {
        name: String::from(""),
        mc_name: None,
        width: 1280,
        height: 720,
        pixfmt: AVPixelFormat::AV_PIX_FMT_NV12,
        align: 0,
        kbs: 1000,
        fps: 30,
        gop: 60,
        quality: Quality_Default,
        rc: RC_CBR,
        thread_count: 4,
        q: -1,
    };
    let start = Instant::now();
    let encoders = Encoder::available_encoders(ctx.clone(), None);
    log::info!("available_encoders:{:?}", start.elapsed());
    log::info!("count:{}, {:?}", encoders.len(), encoders);
    log::info!("best encoders:{:?}", CodecInfo::prioritized(encoders));

    let start = Instant::now();
    let decoders = Decoder::available_decoders(None);
    log::info!("available_decoders:{:?}", start.elapsed());
    log::info!("count:{}, {:?}", decoders.len(), decoders);
    log::info!(
        "best decoders:{:?}",
        CodecInfo::prioritized(decoders.clone())
    );
    log::info!("soft decoders:{:?}", CodecInfo::soft());
}


#[cfg(feature = "vram")]
fn main() {
    init_from_env(Env::default().filter_or(DEFAULT_FILTER_ENV, "trace"));

    println!("encoders:");
    let encoders = encode::available(DynamicContext {
        width: 1920,
        height: 1080,
        kbitrate: 5000,
        framerate: 30,
        gop: MAX_GOP as _,
        device: None,
    });
    encoders.iter().map(|e| println!("{:?}", e)).count();
    println!("decoders:");
    let decoders = decode::available();
    decoders.iter().map(|e| println!("{:?}", e)).count();
}
