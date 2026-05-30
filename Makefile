IMAGE = ffmpeg-image

.PHONY: build run

build:
	docker build -t $(IMAGE) .

run:
	docker run --rm -it $(IMAGE) bash
