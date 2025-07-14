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
}