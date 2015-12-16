package mHelp;

public class JmHelp {
    
    static {
        System.loadLibrary("JmHelp");
    }
    
    /*
     * Main
     */
    public native void Init(long hWnd, String ExeName, boolean Shared);
    public native void Open();
    
    /*
     * Extended
     */
    
}
