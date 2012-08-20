// Objects for parsing BXT structs.

class BXTObject {

    final static int BXT_TYPE_BODY_NONE    = 0;
    final static int BXT_TYPE_BODY_OBJECTS = 1;
    final static int BXT_TYPE_BODY_DATA    = 2;

    public class BXTSize {
        BXTSizeOfSize size_of_size = null;
        BXTData       size         = null;
    }

    public class BXTHeader {
        BXTSize      size      = null;
        BXTPropertys propertys = null;
    }

    public class BXTProperty {
        BXTSize  size  = null;
        BXTKey   key   = null;
        BXTValue value = null;
    }

    public class BXTKey {
        BXTSize  size = null;
        BXTData  key  = null;
    }

    public class BXTValue {
        BXTSize  size  = null;
        BXTData  value = null;
    }

    public class BXTPropertys extend Vector<BXTProperty> {
    }

    public class BXTObjects extend Vector<BXTObject> {
    }

    public class BXTData {
        byte[] data = null;
    }

    public class BXTSizeOfSize {
        int size = 0;
    }

    public static BXTSizeOfSize BXTGetSizeOfSize (BXTData data, BXTSizeOfSize start, BXTSizeOfSize lenght) {
        byte b = data.getByte(start);
        if( b & 11000000b == 11000000b) { // First and second bits iz not, byte include of data.
            return new BXTSizeOfSize(0);
        }
        if(b & 10000000b != 0) { // First bit is not 0, byte include size;
            return new BXTSizeOfSize(1);
        }
        BXTSizeOfSize size = new BXTSizeOfSize(1);
        while(data.getByte(start+size.size) & 10000000b == 0) {
            size.inc();
        }
        size.inc();
        return size;
    }

    public static BXTSize BXTGetSize (BXTData data, BXTSizeOfSize start, BXTSizeOfSize lenght) {
        BXTSizeOfSize size_of_size = BXTGetSizeOfSize(data, start, lenght);
        if(size_of_size.is(0)) {
            return new BXTSize(size_of_size, 0);
        }
        if(size_of_size.is(1)) {
            return new BXTSize(size_of_size, data.getByte(start) & 00111111b);
        }
        return new BXTSize(size_of_size, BXTCopySizeData(data, start, size_of_size);
    }

    public static BXTData BXTCopySizeData (BXTData data, BXTSizeOfSize start, BXTSizeOfSize size) {
        BXTData tmp = new BXTData(BXTGetSizeDataLength(size));
        BXTSizeOfSize j = new BXTSizeOfSize(0);
        for (BXTSizeOfSize i = start+size; i > start; i.dec()) {
            byte b1 = data.getByte(i);

            tmp.insert(, );
        }
        return data.copy(start, end);
    }

    public static BXTSizeOfSize BXTGetSizeDataLength(BXTSizeOfSize size) {
        return size.mul(7).div(8);
    }

    public static BXTData BXTCopyData (BXTData data, BXTSizeOfSize start, BXTSizeOfSize end) {
        return data.copy(start, end);
    }

    public BXTSize    size    = null;
    public BXTHeader  header  = null;
    public BXTObjects objects = null;
    public BXTData    data    = null;
    public int        type    = BXT_TYPE_BODY_NONE;

}
