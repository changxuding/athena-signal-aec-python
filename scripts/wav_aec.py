"""Acoustic Echo Cancellation for wav files."""

import wave
import sys
from athenaaec import EchoCanceller


input_file = "../examples/c_1.wav"
ref_file = "../examples/c_6.wav"
out_file = "../examples/athena_out_1.wav"

frame_size = 128
filter_length =12
near = wave.open(input_file, 'rb')
far = wave.open(ref_file, 'rb')

if near.getnchannels() > 1 or far.getnchannels() > 1:
    print('Only support mono channel')
    sys.exit(2)

out = wave.open(out_file, 'wb')
out.setnchannels(near.getnchannels())
out.setsampwidth(near.getsampwidth())
out.setframerate(near.getframerate())

print('near - rate: {}, channels: {}, length: {}'.format(
        near.getframerate(),
        near.getnchannels(),
        near.getnframes() / near.getframerate()))
print('far - rate: {}, channels: {}'.format(far.getframerate(), far.getnchannels()))



echo_canceller = EchoCanceller.create(128, 16000, 1, 1, 1)

in_data_len = frame_size
in_data_bytes = frame_size * 2
out_data_len = frame_size
out_data_bytes = frame_size * 2

while True:
    in_data = near.readframes(in_data_len)
    out_data = far.readframes(out_data_len)
    if len(in_data) != in_data_bytes or len(out_data) != out_data_bytes:
        break
    processed_data = echo_canceller.process(in_data, out_data)
    out.writeframes(processed_data)

near.close()
far.close()
out.close()
