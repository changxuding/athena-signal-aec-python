# athena-signal-aec-python
Python Wrapper for AEC of Athena Signal
## How to Using This Repository

```
git clone https://github.com/campus-ding/athena-signal-aec-python.git
```
```
make
python3 scripts/wav_aec.py
```
or 
```
sudo python3 setup.py install
python3 wav_aec.py
```

## Parameters
```
EchoCanceller.create(filter_len, sr, input_channel, ref_channel, mode)
```
* mode : 0->common mode, 1-> asr mode

## Reference

* [speexdsp-python](https://github.com/xiongyihui/speexdsp-python)
* [athena-signal](https://github.com/athena-team/athena-signal)
