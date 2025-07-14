#[derive(Clone, Copy, Debug)]
pub struct Dec {
    pub bits: [u32; 4],
}

impl Dec {
    pub const BEGIN: usize = 2;
    pub const METAINFO: usize = 3;

    // Проверяем, является ли число нулём
    pub fn is_zero(&self) -> bool {
        self.bits[0] == 0 && self.bits[1] == 0 && self.bits[Self::BEGIN] == 0
    }

    // Получаем знак числа (true — отрицательное)
    pub fn get_sign(&self) -> bool {
        ((1_u32 << 31) & self.bits[Self::METAINFO]) != 0
    }

    // Получаем масштаб (scale)
    pub fn get_scale(&self) -> u32 {
        (self.bits[Self::METAINFO] >> 16) & 0xFF
    }

    // Получаем бит по позиции (0..=95)
    pub fn get_bit(&self, pos: u32) -> bool {
        let pos = pos.min(95); // Ограничиваем 95

        let part = Self::BEGIN - (pos / 32) as usize; // Определяем, в каком bits[] находится бит
        let local_pos = pos % 32;

        (self.bits[part] >> local_pos) & 1 != 0
    }

    // Установить знак (0 или 1)
    pub fn set_sign(&mut self, sign_value: u32) -> bool {
        if sign_value > 1 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x80000000) | (sign_value << 31);
        false
    }

    // Установить масштаб (от 0 до 28)
    pub fn set_scale(&mut self, scale_value: u32) -> bool {
        if scale_value > 28 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x00FF0000) | (scale_value << 16);
        false
    }

    // Установить бит по позиции (0..=95)
    pub fn set_bit(&mut self, pos: u32, val: bool) -> bool {
        if pos > 95 {
            return true; // ошибка
        }

        let part = Self::BEGIN - (pos / 32) as usize;
        let local_pos = pos % 32;

        if val {
            self.bits[part] |= 1 << local_pos;
        } else {
            self.bits[part] &= !(1 << local_pos);
        }

        false
    }
}

#[derive(Clone, Copy, Debug)]
pub struct BigDec {
    pub bits: [u32; 8],
}

impl BigDec {
    pub const BEGIN: usize = 6;
    pub const METAINFO: usize = 7;

    // Проверяем, является ли большое число нулём
    pub fn is_zero(&self) -> bool {
        self.bits[0] == 0 &&
        self.bits[1] == 0 &&
        self.bits[2] == 0 &&
        self.bits[3] == 0 &&
        self.bits[4] == 0 &&
        self.bits[5] == 0 &&
        self.bits[Self::BEGIN] == 0
    }

    // Получаем знак числа (true — отрицательное)
    pub fn get_sign(&self) -> bool {
        ((1_u32 << 31) & self.bits[Self::METAINFO]) != 0
    }

    // Получаем масштаб (scale)
    pub fn get_scale(&self) -> u32 {
        (self.bits[Self::METAINFO] >> 16) & 0xFF
    }

    // Получаем бит по позиции (0..=223)
    pub fn get_bit(&self, pos: u32) -> bool {
        let pos = pos.min(223); // Ограничиваем 223

        let part = Self::BEGIN - (pos / 32) as usize; // Определяем, в каком bits[] находится бит
        let local_pos = pos % 32;

        (self.bits[part] >> local_pos) & 1 != 0
    }

    // Установить знак (0 или 1)
    pub fn set_sign(&mut self, sign_value: u32) -> bool {
        if sign_value > 1 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x80000000) | (sign_value << 31);
        false
    }

    // Установить масштаб (от 0 до 28)
    pub fn set_scale(&mut self, scale_value: u32) -> bool {
        if scale_value > 28 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x00FF0000) | (scale_value << 16);
        false
    }

    // Установить бит по позиции (0..=223)
    pub fn set_bit(&mut self, pos: u32, val: bool) -> bool {
        if pos > 223 {
            return true; // ошибка
        }

        let part = Self::BEGIN - (pos / 32) as usize;
        let local_pos = pos % 32;

        if val {
            self.bits[part] |= 1 << local_pos;
        } else {
            self.bits[part] &= !(1 << local_pos);
        }

        false
    }
}