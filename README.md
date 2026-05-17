# ffmpeg-liquidsoap

A comprehensive static FFmpeg build published via GitHub Pages as an apt repository. Two packages are provided: static libraries for linking, and CLI tools.

## Package contents

### `ffmpeg-liquidsoap`

- `/usr/local/lib/libav*.a` — static libraries (libavcodec, libavformat, libavutil, libswresample, libswscale, libavfilter)
- `/usr/local/lib/*.a` — all external dependency static archives (bundled, no `-dev` packages needed to link)
- `/usr/local/include/` — headers
- `/usr/local/lib/pkgconfig/` — pkg-config files

### `ffmpeg-liquidsoap-tools`

- `/usr/local/bin/ffmpeg` — static FFmpeg CLI binary
- `/usr/local/bin/ffprobe` — static FFprobe CLI binary

## Supported formats and codecs

All FFmpeg built-in codecs, containers, filters, parsers, and bitstream filters are included. Additionally, the following external libraries are compiled in:

| Feature | Details |
|---------|---------|
| Video encoders | H.264 (libx264), H.265/HEVC (libx265), VP8/VP9 (libvpx), AV1 (SVT-AV1) |
| Video decoders | H.264, H.265/HEVC, VP8, VP9, AV1 (dav1d), + all FFmpeg built-ins |
| Audio encoders | AAC (native + fdk-aac), MP3 (libmp3lame, libshine fixed-point), MP2, FLAC, Vorbis, Opus, PCM variants |
| Audio decoders | AAC, MP3, MP2, FLAC, Vorbis, Opus, PCM variants, + all FFmpeg built-ins |
| Containers | All FFmpeg built-ins: mpegts, matroska/mkv, mp4/mov, ogg, webm, flac, adts, … |
| Protocols | All FFmpeg built-ins: file, pipe, tcp, udp, http, https, rtp, srtp, srt (TLS) |
| Filters | All FFmpeg built-ins: scale, overlay, drawtext (freetype), ass, resample (soxr), … |
| Resampling | High-quality resampling via libsoxr |
| Subtitles | ASS/SSA rendering via libass + libfreetype |

> **Note:** This build uses `--enable-nonfree` (for fdk-aac) and `--enable-gpl` (for x264/x265).
> The resulting libraries are **not redistributable** under GPL to third parties.
> For internal Liquidsoap infrastructure use only.

## Dependency versions (all built from source)

| Library | Version |
|---------|---------|
| FFmpeg | 8.1 |
| OpenSSL | 3.4.0 |
| SRT | 1.5.3 |
| fdk-aac | 2.0.3 |
| lame | 3.100 |
| libogg | 1.3.5 |
| libvorbis | 1.3.7 |
| libopus | 1.5.2 |
| libshine | 3.1.1 |
| x264 | 31e19f92 |
| x265 | 4.1 |
| libvpx | 1.14.1 |
| SVT-AV1 | 2.3.0 |
| dav1d | 1.5.0 |
| libsoxr | 0.1.3 |
| libfreetype | 2.13.3 |
| fribidi | 1.0.15 |
| harfbuzz | 10.1.0 |
| libass | 0.17.3 |
| libFLAC | 1.5.0 |

## Supported architectures

- `amd64` (x86-64)
- `arm64` (AArch64)

## Installing from the apt repository

```sh
curl -fsSL https://liquidsoap.info/ffmpeg-static-build/key.asc \
  | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/liquidsoap-ffmpeg.gpg

echo "deb https://liquidsoap.info/ffmpeg-static-build stable main" \
  | sudo tee /etc/apt/sources.list.d/liquidsoap-ffmpeg.list

sudo apt-get update

# Static libraries and headers (for linking):
sudo apt-get install ffmpeg-liquidsoap

# Static ffmpeg and ffprobe CLI tools (optional):
sudo apt-get install ffmpeg-liquidsoap-tools
```

## Linking against the static libraries

After installing the package, use pkg-config to get the correct flags:

```sh
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

gcc my_program.c -o my_program \
  $(pkg-config --cflags libavcodec libavformat libavutil libswresample) \
  -Wl,--start-group \
  $(pkg-config --libs --static libavcodec libavformat libavutil libswresample) \
  -lpthread -lm -ldl \
  -Wl,--end-group
```

## CI setup

The GitHub Actions workflow builds packages for `amd64` and `arm64`, runs a link test inside a Debian 13 (trixie) container on each architecture, and publishes the apt repository to GitHub Pages.

### Required repository configuration

**GitHub Pages** — enable in *Settings → Pages*, source: *GitHub Actions*.
Or via CLI:
```sh
gh api repos/OWNER/REPO/pages --method POST -f build_type=workflow
```

**GPG signing key** — add as a repository secret named `GPG_PRIVATE_KEY`:
```sh
gpg --gen-key
gpg --armor --export-secret-keys KEY_ID
# paste the output as the secret value
```

**ARM64 runners** — the workflow uses `ubuntu-24.04-arm` GitHub-hosted runners.
Ensure these are available for your organisation/repository.

### Build caching

Each of the 19 external dependencies has its own cache keyed on its version,
so bumping one library triggers only that library's rebuild — all others are
restored from cache. The FFmpeg staging install has a separate cache keyed on
all dependency versions combined.
