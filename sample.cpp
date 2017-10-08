//#include "submit.h"
//#include <unistd.h>
#include <string>
#include <iostream>
#include <queue>
#include <cmath>
#include <stack>
using namespace std;
int ai_side;
string ai_name = "yjybitch";
int MIN = -1000000;
int MAX = 1000000;
pair<int,int>add_choose(0,0);
int rowact[4] = { -2,2,0,0 };
int lineact[4] = { 0,0,2,-2 };
int final_choose = -1;
pair<int, int> ans[3];//�����꣬��12
pair<int, int> maybeact[133];//ʹ��1��132
int value_board[20][20];//for_value_tset remember delete 
class quoridor {
public:
	int board[20][20];
	int rest_of_my_wall;
	int rest_of_an_wall;
	pair<int, int > my_pos;
	pair<int, int > an_pos;
	quoridor() {}
	~quoridor() {}
	pair<int, int> action();
	void update_out(pair<int, int>point);


private:
	struct battleSituation
	{
		int depth;
		int alpha;
		int beta;
		int value;
		struct battlesNode {
			int if_wall;
			int dijkstra_value;
			//pair<int, int> pre;���pre��Ϊ�˺��渳ֵ������
		};
		battlesNode wall_board[20][20];//��˼����Ҫ�ǿ�ǽ�ķŷ���
		pair<int, int> my_now_pos;
		pair<int, int> an_now_pos;
		void dijkstra(pair<int, int> point);
		bool noway();
		int rest_of_my_wall;
		int rest_of_an_wall;
		void addvalue();
		bool knock_wall(int m, int n);
	};
	int gamesort(battleSituation now);
	//4 directions and 17*8 postions setting wall,I choose use stack to save space.
	//the depth is 4 
	battleSituation pre_sort(int m);//����һ�����ڵĻ�����battleSituation
};


void quoridor::update_out(pair<int, int > point)
{
	if (point.first % 2 == 0 && point.second % 2 == 0)
	{
		an_pos = point;
	}
	else if (point.second % 2 == 1)//������ǽ
	{
		board[point.first][point.second] = 1;
		board[point.first + 1][point.second] = 1;
		board[point.first + 2][point.second] = 1;
		rest_of_an_wall -= 1;
	}
	else if (point.first % 2 == 1)//������ǽ
	{
		board[point.first][point.second] = 1;
		board[point.first][point.second + 1] = 1;
		board[point.first][point.second + 2] = 1;
		rest_of_an_wall -= 1;
	}
}


pair<int, int >quoridor::action()
{
	battleSituation tmp = pre_sort(0);
	int fi = gamesort(tmp);
	if (final_choose <= 4)
	{
		pair<int, int>	Ty;
		Ty.first = (my_pos.first + maybeact[final_choose].first);
		Ty.second = (my_pos.second + maybeact[final_choose].second);
		cout << add_choose.first << "  " << add_choose.second << endl;
		if(add_choose.first ==0 && add_choose.second == 0)
		{}
		else{
			cout << "here!";
			Ty.first += add_choose.first;
			Ty.second += add_choose.second;
			add_choose.first = add_choose.second = 0;
		} 
		my_pos = Ty;
		return Ty;
	}
	else if (final_choose >= 5)
	{
		if (maybeact[final_choose].first % 2 == 1)
		{
			board[maybeact[final_choose].first][maybeact[final_choose].second] = 1;
			board[maybeact[final_choose].first][maybeact[final_choose].second + 1] = 1;
			board[maybeact[final_choose].first][maybeact[final_choose].second + 2] = 1;
		}
		else if (maybeact[final_choose].second % 2 == 1)
		{
			board[maybeact[final_choose].first][maybeact[final_choose].second] = 1;
			board[maybeact[final_choose].first + 1][maybeact[final_choose].second] = 1;
			board[maybeact[final_choose].first + 2][maybeact[final_choose].second] = 1;
		}
		rest_of_my_wall -= 1;
		return maybeact[final_choose];
	}
}


bool quoridor::battleSituation::knock_wall(int m, int n)//side��ʾĳһ�����ж�
{
	if (depth == 0)//�ҷ�
	{
		pair<int, int >assume(my_now_pos.first + m / 2, my_now_pos.second + n / 2);
		if (assume.first <= 19 && assume.second <= 19 && assume.second >=1 && assume.first >=1)
		{
			if (wall_board[assume.first][assume.second].if_wall)
				return true;
			else if (wall_board[assume.first][assume.second].if_wall == 0)
				return false;
		}
		return true;
	}
	else
	{
		pair<int, int >assume(an_now_pos.first + m / 2, an_now_pos.second + n / 2);
		if (assume.first <= 19 && assume.second <= 19 && assume.second >=1 && assume.first >=1)
		{
			if (wall_board[assume.first][assume.second].if_wall)
				return true;
			else if (wall_board[assume.first][assume.second].if_wall == 0)
				return false;
		}
		return true;
	}
}

bool quoridor::battleSituation::noway()
{
	int ok1 = 0;
	int ok2 = 0;
	if (ai_side)//��ʾ���������±�
	{
		dijkstra(an_now_pos);
		for (int i = 2; i <= 18; i += 2)
		{
			if (wall_board[18][i].dijkstra_value != 1000)
			{
				ok1 = 1;
				break;
			}
		}
		dijkstra(my_now_pos);
		for (int i = 2; i <= 18; i += 2)
		{
			if (wall_board[2][i].dijkstra_value != 1000)
			{
				ok2 = 1;
				break;
			}
		}
	}
	else if (!ai_side)//������������
	{
		dijkstra(my_now_pos);
		for (int i = 2; i <= 18; i += 2)
		{
			if (wall_board[18][i].dijkstra_value != 1000)
			{
				ok1 = 1;
				break;
			}
		}
		dijkstra(an_now_pos);
		for (int i = 2; i <= 18; i += 2)
		{
			if (wall_board[2][i].dijkstra_value != 1000)
			{
				ok2 = 1;
				break;
			}
		}
	}
	if (ok1 && ok2)
		return  false;
	else return true;
}

void quoridor::battleSituation::dijkstra(pair<int, int> point)//��battleSituation��Ҫ������ͬһ������
{//�ϵ�dev������ģ�����
	for (int i = 2; i < 20; i += 2)//��ʼ��
	{
		for (int j = 2; j < 20; j += 2)
		{
			wall_board[i][j].dijkstra_value = 1000;
		}
	}
	wall_board[point.first][point.second].dijkstra_value = 0;
	queue <pair<int, int > > for_dij;
	for_dij.push(point);
	//dijkstra start
	while (!for_dij.empty())
	{
		pair<int, int >tmp = for_dij.front();
		for_dij.pop();
		for (int i = 0; i < 4; ++i)// 4 directions
		{
			if ((wall_board[tmp.first + rowact[i]][tmp.second + lineact[i]].dijkstra_value == 1000) && (wall_board[tmp.first + rowact[i] / 2][tmp.second + lineact[i] / 2].if_wall == 0))
			{
				wall_board[tmp.first + rowact[i]][tmp.second + lineact[i]].dijkstra_value = wall_board[tmp.first][tmp.second].dijkstra_value + 1;
				pair<int, int> add(tmp.first + rowact[i], tmp.second + lineact[i]);
				for_dij.push(add);
			}
		}
	}
	/*
	for(int i=2;i<=18;i+=2)
	{
	for(int j = 2;j<=18;j+=2)
	{
	cout << wall_board[i][j].dijkstra_value<<'\t';
	}
	cout <<endl;
	}*/
}
void quoridor::battleSituation::addvalue()
{
	value = 0;
	int m[2];
	if (ai_side == 0)//����ʱ��1 
	{
		m[1] = 18;
		m[0] = 2;
	}
	else {
		m[1] = 2;
		m[0] = 18;
	}
	//�������ӵ�״̬����
	dijkstra(my_now_pos);
	for (int i = 2; i < 20; i += 2)
	{//18
		if (wall_board[m[1]][i].dijkstra_value == 1)
		{
			value += 5000;
			continue;
		}
		if (wall_board[m[1]][i].dijkstra_value < 10)
		{
			value += 500 / (wall_board[m[1]][i].dijkstra_value + 1);
		}
	}

	//�з����ӵ�״̬����
	dijkstra(an_now_pos);
	//cout << "an_now_pos"<<an_now_pos.first <<" "<<an_now_pos.second<<endl;
	/*for(int i=2;i<=18;i+=2)
	{
	for(int j = 2;j<=18;j+=2)
	cout << wall_board[i][j].dijkstra_value<<"  ";
	cout <<endl;
	}*/
	//cout << endl;
	for (int i = 2; i < 20; i += 2)
	{
		if (wall_board[m[0]][i].dijkstra_value == 1)
		{
			value -= 5000;
			continue;
		}
		if (wall_board[m[0]][i].dijkstra_value != 1000)
		{
			value -= 700 / (wall_board[m[0]][i].dijkstra_value + 1);
		}
	}
	//ǽ������
	value += 30 / (11 - rest_of_my_wall);
	value -= 30 / (11 - rest_of_an_wall);
}
quoridor::battleSituation quoridor::pre_sort(int m)
{
	battleSituation tmp;
	tmp.value = -3000000;
	tmp.my_now_pos = my_pos;
	tmp.an_now_pos = an_pos;
	tmp.rest_of_an_wall = rest_of_an_wall;
	tmp.rest_of_my_wall = rest_of_my_wall;
	tmp.alpha = MIN;
	tmp.beta = MAX;
	for (int i = 1; i < 20; ++i)
	{
		for (int j = 1; j < 20; ++j)
		{
			tmp.wall_board[i][j].if_wall = board[i][j];
		}
	}
	tmp.depth = m;
	return tmp;
}
int quoridor::gamesort(battleSituation now)//�ݹ�ʵ��DFS��������
{
	if (now.depth == 2)
	{
		//cout << "dep 2 now an pos" <<now.an_now_pos.first <<"  "<< now.an_now_pos.second<<endl;
		now.addvalue();
		return now.value;//������ײ㸳ֵ��105���翪ʼд���¼�֦
	}
	else if (now.depth < 2)
	{
		int floor_value = 0;
		if (now.depth == 1)//�������ǿ���������һ�� floor_value min,������ĵ�״̬���ǵڶ���� 
		{
			floor_value = 100000000;
			for (int j = 1; j <= 132; ++j)
			{
				if (j <= 4)
				{//����ط�������˭��
					//cout <<"j:" << j<<endl;
					if (!now.knock_wall(maybeact[j].first, maybeact[j].second))
					{
						//cout <<"j1:"<<j<<endl;
						//cout << now.an_now_pos.first+maybeact[j].first << endl;
						//cout << now.my_now_pos.first<<"  "<< now.my_now_pos.second << endl;
						if (now.an_now_pos.first + maybeact[j].first != now.my_now_pos.first || now.an_now_pos.second + maybeact[j].second != now.my_now_pos.second)
						{//����ͨ�����
						//cout <<"here"<<endl;
							now.an_now_pos.first += maybeact[j].first;
							now.an_now_pos.second += maybeact[j].second;

							now.depth += 1;
							now.value = gamesort(now);//��ʼ����
							now.depth -= 1;
							now.beta = (now.beta < now.value) ? now.beta : now.value;
							if (floor_value>now.beta)
							{
								floor_value = now.beta;
							}
							now.an_now_pos.first -= maybeact[j].first;
							now.an_now_pos.second -= maybeact[j].second;
						}
						else if(now.my_now_pos.first + maybeact[j].first == now.an_now_pos.first && now.my_now_pos.second + maybeact[j].second == now.an_now_pos.second)
						{//ײ����
							if (!now.knock_wall(maybeact[j].first * 2, maybeact[j].second * 2))
							{//����ûǽ
								now.an_now_pos.first += maybeact[j].first;
								now.an_now_pos.first += maybeact[j].first;
								now.an_now_pos.second += maybeact[j].second;
								now.an_now_pos.second += maybeact[j].second;
								now.depth += 1;
								now.value = gamesort(now);
								now.depth -= 1;
								//����ط������ݻ�����ʱ��Ӧ����beta�����Ƕ����²��Ծ���alpha
								now.beta = (now.beta < now.value) ? now.beta : now.value;
								if (floor_value > now.beta)
								{
									floor_value = now.beta;
								}
								now.an_now_pos.first -= maybeact[j].first;
								now.an_now_pos.first -= maybeact[j].first;
								now.an_now_pos.second -= maybeact[j].second;
								now.an_now_pos.second -= maybeact[j].second;
							}
							else if(now.knock_wall(maybeact[j].first * 2, maybeact[j].second * 2))//������ǽ 
							{//�������۱�����ǽ��������������涼û���ߵ������ת
								now.an_now_pos.first += maybeact[j].first;
								now.an_now_pos.second += maybeact[j].second;
								if (!now.knock_wall(maybeact[j].second, maybeact[j].first))
								{
									now.an_now_pos.first += maybeact[j].second;
									now.an_now_pos.second += maybeact[j].first;
									now.depth += 1;
									now.value = gamesort(now);
									now.depth -= 1;
									now.beta = (now.beta < now.value) ? now.beta : now.value;
									if (floor_value > now.beta)
									{
										floor_value = now.beta;
									}
									now.an_now_pos.first -= maybeact[j].first;
									now.an_now_pos.first -= maybeact[j].second;
									now.an_now_pos.second -= maybeact[j].second;
									now.an_now_pos.second -= maybeact[j].first;
								}
								else if (!now.knock_wall(-maybeact[j].second, -maybeact[j].first))
								{
									now.an_now_pos.first -= maybeact[j].second;
									now.an_now_pos.second -= maybeact[j].first;
									now.depth += 1;
									now.value = gamesort(now);
									now.depth -= 1;
									now.beta = (now.beta < now.value) ? now.beta : now.value;
									if (floor_value > now.beta)
									{
										floor_value = now.beta;
									}
									now.an_now_pos.first -= maybeact[j].first;
									now.an_now_pos.first += maybeact[j].second;
									now.an_now_pos.second -= maybeact[j].second;
									now.an_now_pos.second += maybeact[j].first;
								}
							}
						}
					}
				}
				//�����Ƿ�ǽ������
				else if (j >= 5)
				{
					if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0 && now.rest_of_an_wall >0)
					{
						if (maybeact[j].first % 2)//����3��2����
						{
							if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0)
							{
								bool b,a;
								b = false; a= false;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first][maybeact[j].second + 1].if_wall == 1)
									a = true;
								now.wall_board[maybeact[j].first][maybeact[j].second + 1].if_wall = 1;
								if(now.wall_board[maybeact[j].first][maybeact[j].second+2].if_wall == 1)
									b = true;//������������ǽ 
								now.wall_board[maybeact[j].first][maybeact[j].second + 2].if_wall = 1;
								now.rest_of_an_wall -= 1;
								now.depth += 1;
								if (!now.noway())
									now.value = gamesort(now);//����״̬
								else
								{
									now.value = -300000;
								}
								now.rest_of_an_wall += 1;
								now.beta = (now.beta < now.value) ? now.beta : now.value;
								if (floor_value > now.beta)
								{
									floor_value = now.beta;
								}
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 0;
								if(!a)
									now.wall_board[maybeact[j].first][maybeact[j].second + 1].if_wall = 0;
								if (!b)
								{
									now.wall_board[maybeact[j].first][maybeact[j].second + 2].if_wall = 0;
								}
								now.depth -= 1;
							}
						}
						else if (maybeact[j].first % 2 == 0)//2��3����
						{
							if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0)
							{
								bool a,b;
								a = false; b = false;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first + 1][maybeact[j].second].if_wall == 1)
									a = true;
								now.wall_board[maybeact[j].first + 1][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first+2][maybeact[j].second].if_wall == 1)
									b = true;
								now.wall_board[maybeact[j].first + 2][maybeact[j].second].if_wall = 1;
								now.rest_of_an_wall -= 1;
								now.depth += 1;
								if (!now.noway())
									now.value = gamesort(now);//����
								else
								{
									now.value = -300000;
								}
								now.beta = (now.beta < now.value) ? now.beta : now.value;
								if (floor_value > now.beta)
								{
									floor_value = now.beta;
								}
								now.rest_of_an_wall += 1;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 0;
								if(!a)
									now.wall_board[maybeact[j].first + 1][maybeact[j].second].if_wall = 0;
								if (!b)
									now.wall_board[maybeact[j].first + 2][maybeact[j].second].if_wall = 0;
								now.depth -= 1;
							}
						}
					}
				}
			}
			now.depth -= 1;
			return floor_value;
		}
		else if (now.depth == 0)//�������ǿ���0 floor max�����ĵĶ��ǵ�һ���״̬ 
		{
			floor_value = -300000;
			for (int j = 1; j <= 132; ++j)
			{
				if (j <= 4)
				{//����ط�������˭��
					cout << "an pos1:" << now.an_now_pos.first << ' ' << now.an_now_pos.second << endl;
					if (!now.knock_wall(maybeact[j].first, maybeact[j].second))
					{
						if (now.my_now_pos.first + maybeact[j].first != now.an_now_pos.first || now.my_now_pos.second + maybeact[j].second != now.an_now_pos.second)
						{//����ͨ�����
							now.my_now_pos.first += maybeact[j].first;
							now.my_now_pos.second += maybeact[j].second;
							now.depth += 1;
							now.value = gamesort(now);//��ʼ���� get floor_value;
							now.depth -= 1;
							now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
							value_board[now.my_now_pos.first][now.my_now_pos.second] = now.value;
							if (now.alpha >floor_value)
							{
								floor_value = now.alpha;
								final_choose = j;
							}
							now.value = 0;
							now.my_now_pos.first -= maybeact[j].first;
							now.my_now_pos.second -= maybeact[j].second;
						}
						else if(now.my_now_pos.first + maybeact[j].first == now.an_now_pos.first && now.my_now_pos.second + maybeact[j].second == now.an_now_pos.second)
						{//ײ����
							cout << "my pos:" << now.my_now_pos.first << ' ' << now.my_now_pos.second << endl;
							cout << "an pos:" << now.an_now_pos.first << ' ' << now.an_now_pos.second << endl;
							cout << "maybeact:" << maybeact[j].first << ' ' << maybeact[j].second << endl;
							now.my_now_pos.first += maybeact[j].first;
							now.my_now_pos.second += maybeact[j].second;
							if (!now.knock_wall(maybeact[j].first , maybeact[j].second))
							{//����ûǽ
								now.my_now_pos.first += maybeact[j].first;
								now.my_now_pos.second += maybeact[j].second;
								add_choose.first = maybeact[j].first;
								add_choose.second = maybeact[j].second;
								now.depth += 1;
								now.value = gamesort(now);
								//����ط������ݻ�����ʱ��Ӧ����beta�����Ƕ����²��Ծ���alpha
								now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
								value_board[now.my_now_pos.first][now.my_now_pos.second] = now.value;
								add_choose.first = add_choose.second = 0;
								if (now.alpha >floor_value)
								{
									floor_value = now.alpha;
									final_choose = j;
									add_choose.first = maybeact[j].first;
									add_choose.second = maybeact[j].second;
								}
								now.value = 0;
								now.my_now_pos.first -= maybeact[j].first;
								now.my_now_pos.first -= maybeact[j].first;
								now.my_now_pos.second -= maybeact[j].second;
								now.my_now_pos.second -= maybeact[j].second;
								now.depth -= 1;
							}
							else if(now.knock_wall(maybeact[j].first, maybeact[j].second))//������ǽ
							{//�������۱�����ǽ��������������涼û���ߵ������ת
								if (!now.knock_wall(maybeact[j].second, maybeact[j].first))
								{
									now.my_now_pos.first += maybeact[j].second;
									now.my_now_pos.second += maybeact[j].first;
									add_choose.first = maybeact[j].second;
									add_choose.second = maybeact[j].first;
									now.depth += 1;
									now.value = gamesort(now);
									value_board[now.my_now_pos.first][now.my_now_pos.second] = now.value;
									now.depth -= 1;
									now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
									add_choose.first = add_choose.second = 0;
									if (now.alpha >floor_value)
									{
										floor_value = now.alpha;
										final_choose = j;
										add_choose.first = maybeact[j].second;
										add_choose.second = maybeact[j].first;
									}
									now.my_now_pos.first -= maybeact[j].first;
									now.my_now_pos.first -= maybeact[j].second;
									now.my_now_pos.second -= maybeact[j].second;
									now.my_now_pos.second -= maybeact[j].first;
								}
								else if (!now.knock_wall(-maybeact[j].second, -maybeact[j].first))
								{
									now.my_now_pos.first -= maybeact[j].second;
									now.my_now_pos.second -= maybeact[j].first;
									add_choose.first = -maybeact[j].second;
									add_choose.second = -maybeact[j].first;
									now.depth += 1;
									now.value = gamesort(now);
									add_choose.first = 0;
									add_choose.second = 0;
									value_board[now.my_now_pos.first][now.my_now_pos.second] = now.value;
									now.depth -= 1;
									now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
									if (now.alpha >floor_value)
									{
										floor_value = now.alpha;
										final_choose = j;
										add_choose.first = -maybeact[j].second;
										add_choose.second = -maybeact[j].first;
									}
									now.my_now_pos.first -= maybeact[j].first;
									now.my_now_pos.first += maybeact[j].second;
									now.my_now_pos.second -= maybeact[j].second;
									now.my_now_pos.second += maybeact[j].first;
								}
							}
						}
					}
				}
				//�����Ƿ�ǽ������
				else if (j >= 5)
				{
					if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0 && now.rest_of_my_wall >0)
					{
						if (maybeact[j].first % 2 == 1)//����3��2����
						{
							if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0)
							{
								bool a,b;
								a = false;b=false;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first][maybeact[j].second +1].if_wall == 1)
									a = true;
								now.wall_board[maybeact[j].first][maybeact[j].second + 1].if_wall = 1;
								if(now.wall_board[maybeact[j].first][maybeact[j].second +2].if_wall == 1)
									b = true;
								now.wall_board[maybeact[j].first][maybeact[j].second + 2].if_wall = 1;
								now.rest_of_my_wall -= 1;
								now.depth += 1;
								if (!now.noway())
									now.value = gamesort(now);//����״̬
								else
								{
									now.value = -300000;
								}
								value_board[maybeact[j].first][maybeact[j].second] = now.value;
								now.rest_of_my_wall += 1;
								now.depth -= 1;
								now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
								if (now.alpha >floor_value)
								{
									floor_value = now.alpha;
									final_choose = j;
								}
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 0;
								if(!a)
									now.wall_board[maybeact[j].first][maybeact[j].second + 1].if_wall = 0;
								if (!b)
									now.wall_board[maybeact[j].first][maybeact[j].second + 2].if_wall = 0;
							}
						}
						else if (maybeact[j].first % 2 == 0)//2��3����
						{
							if (now.wall_board[maybeact[j].first][maybeact[j].second].if_wall == 0)
							{
								bool a,b;
								a = false;b = false;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first+1][maybeact[j].second].if_wall == 1)
									a = true;
								now.wall_board[maybeact[j].first + 1][maybeact[j].second].if_wall = 1;
								if(now.wall_board[maybeact[j].first+2][maybeact[j].second].if_wall == 1)
									b = true;
								now.wall_board[maybeact[j].first + 2][maybeact[j].second].if_wall = 1;
								now.depth += 1;
								now.rest_of_my_wall -= 1;
								if (!now.noway())
									now.value = gamesort(now);//����
								else
								{
									now.value = -300000;
								}
								now.depth -= 1;
								value_board[maybeact[j].first][maybeact[j].second] = now.value;
								now.alpha = (now.alpha > now.value) ? now.alpha : now.value;
								if (now.alpha >floor_value)
								{
									floor_value = now.alpha;
									final_choose = j;
								}
								now.rest_of_my_wall += 1;
								now.wall_board[maybeact[j].first][maybeact[j].second].if_wall = 0;
								if(!a)
									now.wall_board[maybeact[j].first + 1][maybeact[j].second].if_wall = 0;
								if (!b)
									now.wall_board[maybeact[j].first + 2][maybeact[j].second].if_wall = 0;
							}
						}
					}
				}
			}
			return floor_value;
		}

	}
}

quoridor alls;

void init()
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			alls.board[i][j] = 0;
		}
	}
	for (int i = 1; i < 20; ++i)
	{
		alls.board[i][1] = 1;
		alls.board[19][i] = 1;
		alls.board[i][19] = 1;
		alls.board[1][i] = 1;//�߽���ǽ
	}
	alls.rest_of_an_wall = 10;
	alls.rest_of_my_wall = 10;
	if (ai_side == 1)
	{
		alls.my_pos.first = 18;
		alls.my_pos.second = 10;
		alls.an_pos.first = 2;
		alls.an_pos.second = 10;
	}
	else if (ai_side == 0)
	{
		alls.my_pos.first = 2;
		alls.my_pos.second = 10;
		alls.an_pos.first = 18;
		alls.an_pos.second = 10;
	}
	for (int i = 1; i <= 4; ++i)
	{
			maybeact[i].first = rowact[i - 1];
			maybeact[i].second = lineact[i - 1];
	}
	for(int i =5;i<=132;++i)
	{
		for (int m = 2; m < 18; m += 2)
			{
				for (int n = 3; n < 18; n += 2)
				{
					maybeact[i].first = m;
					maybeact[i].second = n;
					++i;
				}
			}
			for (int m = 3; m < 18; m += 2)
			{
				for (int n = 2; n < 18; n += 2)
				{
					maybeact[i].first = m;
					maybeact[i].second = n;
					++i;
				}
			}
	}
}
	
void GetUpdate(pair<int, int > m)
{
	alls.update_out(m);
}
pair<int, int >Action()
{
	return alls.action();
}
int main()
{
	ai_side = 1;
	init();
	pair<int, int>father;
	father.first = 18;
	father.second = 10;
	for (int m = 1; m <= 18; ++m)
	{
		for (int n = 1; n <= 18; ++n)
		{
			value_board[m][n] = 0;
		}
	}
	while (father.first != -1)
	{
		int x, y;
		cin >> x >> y;
		father.first = x;
		father.second = y;
		GetUpdate(father);
		pair<int, int> my = Action();
		cout <<final_choose<<endl;
		cout << my.first << "  " << my.second << endl;
		final_choose = -1;
	}
}

