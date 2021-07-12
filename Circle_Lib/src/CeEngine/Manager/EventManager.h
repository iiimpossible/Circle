#include <iostream>
#include "../../CCCommon/Tool/Delegate.h"

using namespace CircleEngine;


/**
 * @brief ������¼������࣬Ŀ���ǹ�����־����ȣ����������Ӧ���Ǹ�������ײ㣨��ͼ����Ƶ�Ȼ���ģ�飩����Ϊ���û�
 * ���潻�����࣬����������־������̨�������Ҳ��ܽ������Ƕ�뵽��ͼAPI����ɫ�����룩��ȥ��Ӧ�ý�������÷ŵ�Render��
 */
class EventManager
{
public:
	EventManager();

	~EventManager();	

	//��Ҫ��Ϊ��̬�����¼���
	static Delegate<void,int> OnShaderCompileError;

	//Delegate<int,int> OnPathError;

private:

	void ShaderCompileError(int);

	int PathError(int);
	
	

};