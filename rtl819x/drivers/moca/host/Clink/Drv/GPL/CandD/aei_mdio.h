#ifndef AEI_MDIO_H_
#define AEI_MDIO_H_

#define K_BUF_SIZE   512
#define CANDD_PHYID  0x5

#define reverse_endian(b)       ((b) >= 8 ? (b)+8 : (b)+24)
#define reverse_endian_bit(b)   (1 << reverse_endian(b))

#define MDIO_START_BITS         (0x01 << (30-16))
#define MDIO_OP_READ            (0x02 << (28-16))
#define MDIO_OP_WRITE           (0x01 << (28-16))
#define MDIO_READ_TURN_AROUND   (0x03 << (16-16))
#define MDIO_WRITE_TURN_AROUND  (0x02 << (16-16))

#define MDIO_READ_MASK          (MDIO_START_BITS | MDIO_OP_READ  | PHY_ADDRESS << (23-16))
#define MDIO_WRITE_MASK         (MDIO_START_BITS | MDIO_OP_WRITE | PHY_ADDRESS << (23-16))  
#define MDIO_REG_PLACEMENT      (18-16)

#define ADDRESS_MODE            0x1b
#define ADDRESS_HIGH            0x1c
#define ADDRESS_LOW             0x1d
#define DATA_HIGH               0x1e
#define DATA_LOW                0x1f

#define _BV(n)                  (1 << (n))

#define CLINK_START_WRITE       _BV(0)
#define CLINK_START_READ        _BV(1)
#define CLINK_AUTO_INC          _BV(2)
#define CLINK_BUSY              _BV(3)
#define CLINK_ERROR             _BV(4)

#define __PHY_ADDRESS           0x10

#define PHY_READ    (MDIO_START_BITS | MDIO_OP_READ  | MDIO_READ_TURN_AROUND)
#define PHY_WRITE   (MDIO_START_BITS | MDIO_OP_WRITE | MDIO_WRITE_TURN_AROUND)

#endif //AEI_MDIO_H_
