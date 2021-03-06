import wx
import os

class NewProjectDialog(wx.Dialog):
    def __init__(self, okayhandle, cancelhandle, ide):
        # begin wxGlade: MyDialog3.__init__
##        kwds["style"] = wx.DEFAULT_DIALOG_STYLE
        wx.Dialog.__init__(self, ide, style=wx.DEFAULT_DIALOG_STYLE)
        self.bitmap_button_1 = wx.BitmapButton(self, -1, wx.Bitmap(ide.appdir+"/Data/images/dlg_newproject.png", wx.BITMAP_TYPE_ANY), style=wx.NO_BORDER)
        self.lblInformation = wx.StaticText(self, -1, " Enter the name of the new project:")
        self.text_ctrl_1 = wx.TextCtrl(self, -1, "")
        self.button_2 = wx.Button(self, wx.ID_CANCEL, "")
        self.button_1 = wx.Button(self, wx.ID_OK, "")
        self.button_1.SetDefault()

        self.__set_properties()
        self.__do_layout()

        self.okayButtonCaller = okayhandle
        self.cancelButtonCaller = cancelhandle

        wx.EVT_BUTTON(self, self.button_2.GetId(), self.cancelCalled)
        wx.EVT_BUTTON(self, self.button_1.GetId(), self.okayCalled)

        self.CentreOnScreen()
        self.text_ctrl_1.SetFocus()
        
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: MyDialog3.__set_properties
        self.SetTitle("New Project")
        self.SetSize((517, 313))
        self.bitmap_button_1.SetSize((501,201))
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: MyDialog3.__do_layout
        sizer_2 = wx.BoxSizer(wx.VERTICAL)
        sizer_3 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_2.Add(self.bitmap_button_1, 0, wx.LEFT|wx.RIGHT|wx.TOP|wx.EXPAND|wx.SHAPED, 5)
        sizer_2.Add(self.lblInformation, 0, wx.LEFT|wx.RIGHT|wx.TOP, 5)
        sizer_2.Add(self.text_ctrl_1, 0, wx.LEFT|wx.RIGHT|wx.TOP|wx.EXPAND, 5)
        sizer_3.Add((300, 20), 1, wx.EXPAND, 0)
        sizer_3.Add(self.button_2, 0, wx.LEFT|wx.TOP|wx.BOTTOM|wx.ALIGN_RIGHT, 5)
        sizer_3.Add(self.button_1, 0, wx.ALL|wx.ALIGN_RIGHT|wx.FIXED_MINSIZE, 5)
        sizer_2.Add(sizer_3, 1, wx.EXPAND|wx.ALIGN_RIGHT, 0)
        self.SetSizer(sizer_2)
        self.Layout()
        # end wxGlade

    def cancelCalled(self, event):
        self.cancelButtonCaller()
        self.EndModal(0)

    def okayCalled(self, event):
        if (self.text_ctrl_1.GetValue() != ""):
            self.okayButtonCaller(self.text_ctrl_1.GetValue())
            self.EndModal(0)
        else:
            dlg = wx.MessageDialog(self, "Please enter a project name!","Error",wx.OK)
            dlg.ShowModal()
