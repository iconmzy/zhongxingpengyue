
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
int current_port = 0;
bool first_full = true;
struct Flow {
    int id; // �����
    int bandwidth; // ������
    int arrival_time; // �����豸ʱ��
    int send_time; // ����������ʱ��
    int port_id; // ����Ķ˿ڱ��
    int start_time; // ���Ŀ�ʼ����ʱ��
};
struct Port {
    int id; // �˿ڱ��
    int bandwidth; // �˿ڴ���
    int used_bw; // ��ʹ�õĴ���
    int current_time;
    int free_time;

};
bool cmp(const Flow& f1, const Flow& f2) {
    //������Ӧ������
    return (double)(f1.bandwidth * f1.bandwidth) / f1.send_time <
           (double)(f2.bandwidth * f2.bandwidth) / f2.send_time;
}





void reset_ports(vector<Port>& ports) {
    for (int i = 0; i < ports.size(); i++) {

            ports[i].used_bw =  0;
            ports[i].current_time = ports[i].free_time;


    }


}




int main() {
    string root_folder = "..//data/"; // ��Ŀ¼Ϊdata�ļ���
    int folder = 0;
    // ����ÿ�����ļ��в�������д���
    while (true) {
        string current_port_file = root_folder + "/" + to_string(folder) + "/port.txt";
        string current_flow_file = root_folder + "/" + to_string(folder) + "/flow.txt";
        string current_result_file = root_folder + "/" + to_string(folder) + "/result.txt";
    // ��ȡ�˿���Ϣ
    ifstream port_file(current_port_file);
        if (!port_file) {
            return 0;
        }
    string line;
    getline(port_file, line); // ������һ��
    vector<Port> ports;
    while (getline(port_file, line)) {
        Port p;
        sscanf(line.c_str(), "%d,%d", &p.id, &p.bandwidth);
        p.used_bw = 0;
        p.current_time = 0;
        ports.push_back(p);
    }
    port_file.close();
    // ��ȡ����Ϣ
    ifstream flow_file(current_flow_file);
    getline(flow_file, line); // ������һ��
    vector<Flow> flows;
    while (getline(flow_file, line)) {
        Flow f;
        sscanf(line.c_str(), "%d,%d,%d,%d", &f.id, &f.bandwidth, &f.arrival_time, &f.send_time);
        f.port_id = -1;
        f.start_time = -1;
        flows.push_back(f);
    }
    flow_file.close();

    sort(flows.begin(), flows.end(), cmp);



    while (!flows.empty()) {
        Flow &f = flows.front();
        int port_available = 0;
        int temp_current_port = current_port;
        for(int current = 0;current <=7;current++){
            if(ports[temp_current_port].used_bw + f.bandwidth <= ports[temp_current_port].bandwidth){
                port_available = 1;
                current_port= temp_current_port;
                break;
            }else{
                temp_current_port = (temp_current_port+1)%ports.size();
            }
        }
        if (port_available) {
            f.port_id = ports[current_port].id;
            ports[current_port].used_bw += f.bandwidth;
            f.start_time = max(ports[current_port].current_time, f.arrival_time);
            ports[current_port].free_time = max(ports[current_port].free_time,(f.start_time+f.send_time));

            // �����������ļ�
            ofstream output_file(current_result_file,std::ios::app);
            output_file << f.id << "," << f.port_id << "," << f.start_time << endl;
            output_file.close();

            flows.erase(flows.begin());


        }else{
            // û�п��ö˿ڣ�������ӵ��ȴ�����
            reset_ports(ports);
        }
        current_port = (current_port+1)%ports.size();

    }

    folder++;
    current_port = 0;
    }




    return 0;
}
