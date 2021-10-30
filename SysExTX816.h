
class CSysExTX816
{
	CMidiOutDevice* m_pOutPort;
public:
	CSysExTX816();
	~CSysExTX816();
	void Create(CMidiOutDevice* pOutPort);
	void TX816_VoiceBulkData(int channel, UCHAR* pData);
	void TX816_PerformanceBulkData(int channel, UCHAR* pData);
	void TX816_PerformanceBulkData64(int channel, UCHAR* pData);
	void TX816_VoiceBulkData32(int channel, UCHAR* pData);
	void TX816_ParameterChange(int channel, int groupNumber, int subgroup, int parameterNumber, int parameter);
	void DumpRequest(int chan, int format);
};
