// adapted from Adafruit library for DRV2605 motor driver!
const EventEmitter = require('events');

const DRV2605_ADDR = 0x5A;

const DRV2605_REG_STATUS      = 0x00,
      DRV2605_REG_MODE        = 0x01,
      DRV2605_MODE_INTTRIG    = 0x00,
      DRV2605_MODEEXTTRIGEDGE = 0x01,
      DRV2605_MODE_EXTTRIGLVL = 0x02,  ///< External level trigger mode
      DRV2605_MODE_PWMANALOG  = 0x03, ///< PWM/Analog input mode
      DRV2605_MODE_AUDIOVIBE  = 0x04,   ///< Audio-to-vibe mode
      DRV2605_MODE_REALTIME   = 0x05,    ///< Real-time playback (RTP) mode
      DRV2605_MODE_DIAGNOS    = 0x06,     ///< Diagnostics mode
      DRV2605_MODE_AUTOCAL    = 0x07,
      DRV2605_REG_RTPIN       = 0x02,    ///< Real-time playback input register
      DRV2605_REG_LIBRARY     = 0x03,  ///< Waveform library selection register
      DRV2605_REG_WAVESEQ1    = 0x04, ///< Waveform sequence register 1
      DRV2605_REG_WAVESEQ2    = 0x05, ///< Waveform sequence register 2
      DRV2605_REG_WAVESEQ3    = 0x06, ///< Waveform sequence register 3
      DRV2605_REG_WAVESEQ4    = 0x07, ///< Waveform sequence register 4
      DRV2605_REG_WAVESEQ5    = 0x08, ///< Waveform sequence register 5
      DRV2605_REG_WAVESEQ6    = 0x09, ///< Waveform sequence register 6
      DRV2605_REG_WAVESEQ7    = 0x0A, ///< Waveform sequence register 7
      DRV2605_REG_WAVESEQ8    = 0x0B, ///< Waveform sequence register 8
      DRV2605_REG_GO          = 0x0C,         ///< Go register
      DRV2605_REG_OVERDRIVE   = 0x0D,  ///< Overdrive time offset register
      DRV2605_REG_SUSTAINPOS  = 0x0E, ///< Sustain time offset, positive register
      DRV2605_REG_SUSTAINNEG  = 0x0F, ///< Sustain time offset, negative register
      DRV2605_REG_BREAK       = 0x10,      ///< Brake time offset register
      DRV2605_REG_AUDIOCTRL   = 0x11,  ///< Audio-to-vibe control register
      DRV2605_REG_AUDIOLVL    = 0x12, ///< Audio-to-vibe minimum input level register
      DRV2605_REG_AUDIOMAX    = 0x13, ///< Audio-to-vibe maximum input level register
      DRV2605_REG_AUDIOOUTMIN = 0x14, ///< Audio-to-vibe minimum output drive register
      DRV2605_REG_AUDIOOUTMAX = 0x15,                          ///< Audio-to-vibe maximum output drive register
      DRV2605_REG_RATEDV      = 0x16, ///< Rated voltage register
      DRV2605_REG_CLAMPV      = 0x17, ///< Overdrive clamp voltage register
      DRV2605_REG_AUTOCALCOMP = 0x18, ///< Auto-calibration compensation result register
      DRV2605_REG_AUTOCALEMP  = 0x19,                            ///< Auto-calibration back-EMF result register
      DRV2605_REG_FEEDBACK    = 0x1A, ///< Feedback control register
      DRV2605_REG_CONTROL1    = 0x1B, ///< Control1 Register
      DRV2605_REG_CONTROL2    = 0x1C, ///< Control2 Register
      DRV2605_REG_CONTROL3    = 0x1D, ///< Control3 Register
      DRV2605_REG_CONTROL4    = 0x1E, ///< Control4 Register
      DRV2605_REG_VBAT        = 0x21,     ///< V    bat voltage-monitor register
      DRV2605_REG_LRARESON    = 0x22;

class DRV2605 extends EventEmitter {
    constructor(device) {

        super();

        this.address = DRV2605_ADDR;
        this.device = device;

        this.ready = false;
        this.init();
    }

    init() {
        console.log("drv2605: init()");

        return new Promise((resolve, reject) => {
            this.writeRegister8(DRV2605_REG_MODE, 0x00)
            .then(this.writeRegister8(DRV2605_REG_RTPIN, 0x00))
            .then(this.writeRegister8(DRV2605_REG_WAVESEQ1, 1))
            .then(this.writeRegister8(DRV2605_REG_WAVESEQ2, 0))
            .then(this.writeRegister8(DRV2605_REG_OVERDRIVE, 0))
            .then(this.writeRegister8(DRV2605_REG_SUSTAINPOS, 0))
            .then(this.writeRegister8(DRV2605_REG_SUSTAINNEG, 0))
            .then(this.writeRegister8(DRV2605_REG_BREAK, 0))
            .then(this.writeRegister8(DRV2605_REG_AUDIOMAX, 0x64))
            .then(this.writeRegister8(DRV2605_REG_FEEDBACK, this.readRegister8(DRV2605_REG_FEEDBACK) & 0x7F))
            .then(this.writeRegister8(DRV2605_REG_CONTROL3, this.readRegister8(DRV2605_REG_CONTROL3) | 0x20))
            .then(() => {
                this.ready = true;
                this.emit('ready');
              })
            .catch((err) => {
            console.log(err);
            this.emit('error', err);
            reject(err);
            });
        })
    }

    setWaveform(slot, w) { // both uint8_t
        this.writeRegister8(DRV2605_REG_WAVESEQ1 + slot, w).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    selectLibrary(lib) { // uint8_t
        this.writeRegister8(DRV2605_REG_LIBRARY, lib).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    go() {
        this.writeRegister8(DRV2605_REG_GO, 1).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    // not used
    stop() {
        this.writeRegister8(DRV2605_REG_GO, 0).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    // not used
    setMode(mode) { // uint8_t
        this.writeRegister8(DRV2605_REG_MODE, mode).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    // not used
    setRealtimeValue(rtp) { // uint8_t
        this.writeRegister8(DRV2605_REG_RTPIN, rtp).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    readRegister8(reg) {
        return new Promise((resolve, reject) => {

            this.device.readByte(this.address, reg, (err, b) => {
              if(err) return reject(err);
              resolve(b);
            });
        });
    }

    writeRegister8(reg, val) {
        return new Promise((resolve, reject) => {
            this.device.writeByte(this.address, reg, val & 0xFF, (err) => { // bitwise thingy, keeps it a byte or less i think
              if(err) return reject(err);
              resolve();
            });
        });
    }

    useERM() {
        this.writeRegister8(DRV2605_REG_FEEDBACK, this.readRegister8(DRV2605_REG_FEEDBACK) & 0x7F).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }

    useLRA() {
        this.writeRegister8(DRV2605_REG_FEEDBACK, this.readRegister8(DRV2605_REG_FEEDBACK) | 0x80).catch((err) => {
            console.log(err);
            this.emit('error', err);
            });
    }
}

exports = module.exports = DRV2605;
