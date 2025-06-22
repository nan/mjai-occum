#ifndef _OPM_
#define _OPM_
#include "ma_jan.h"
#include "../util.h"

#define simtime 1000

struct player
{
 int fu_ro_suu;
 int fu_ro[4];
 int aka_fu_ro;
 int aka_moti;
 int ri_ti_flag;
 int ri_ti_sute;
 int fu_ro_sute[4];
 int ippatu;
 int tenho;
       
 int tensuu;
 vector<int> sutehai;//37
 vector<int> tedasi;
 int tegawari[34];
 int genbutu[34];
 int hai[34];
 int jifuu;
 int fold_flag;
};
struct field
{
 int bafuu;
 int kyoku;
 int game_long;
 int ri_ti_bou;
 int honba;
 int dora[5];
 int dora_num;
 int haitei;
 int kan_flag;

 int dora_basho;
 int nokori_maisuu;
 int sutehai_sarasi[37];
 int mieta_aka_hai;
};



class BKUC1681 {
public:

	double sim_threshold;

	//int *op_list;
	//int op_list[util.IntToString(99999999];

	boost::unordered_map<std::string, int> op_list;
	
	//std::map<std::string, int> op_list;

	//extern double sutehai_pro[34][5][34];
	double n_seme_pro[10][4096][18];

	double **tenpai_wei;
	double **matiyomi_wei;
	double *tokuten_wei;

	BKUC1681MajhanUtil util;

	//tenpai_pre
	boost::unordered_map<std::string, int> tenpai_pre_list;
	boost::unordered_map<std::string, int> tenpai_train_list;

	int init_tenpai_pre_weight()
	{
		string str;
		int sum = 0;
		// std::ofstream ofs("/data/local/mizukami/tenpai_pre/tenpai_pre_tag.txt");
		for (int i = 0; i <= 1; i++)
		{
			sum++;
			str = "r" + util.IntToString(i);
			tenpai_train_list[str] = sum;
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 18; j++)
			{
				sum++;
				str = "f" + util.IntToString(i) + "sute" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 0; j <= 18; j++)
			{
				sum++;
				str = "f" + util.IntToString(i) + "junme" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 18; j++)
			{
				sum++;
				str = "f" + util.IntToString(i) + "all_tds" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 18; j++)
			{
				sum++;
				str = "f" + util.IntToString(i) + "tds" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j<37; j++)
			{
				sum++;
				str = "f" + util.IntToString(i) + "last_tds" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 0; i<136; i++)
		{
			for (int j = 0; j<37; j++)
			{
				sum++;
				str = "f_k" + util.IntToString(i) + "tds" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 0; i<34; i++)//dora
		{
			sum++;
			str = "d" + util.IntToString(i);
			tenpai_train_list[str] = sum;
		}

		{
			sum++;
			str = "aka";
			tenpai_train_list[str] = sum;
		}
		for (int i = 0; i<37; i++)
		{
			for (int j = 0; j<37; j++)
			{
				sum++;
				str = "sute" + util.IntToString(i) + "_" + util.IntToString(j);
				tenpai_train_list[str] = sum;
			}
		}
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				for (int k = j; k <= 2; k++)
				{
					for (int l = 0; l <= 18; l++)
					{
						sum++;
						str = "some_f" + util.IntToString(i) + "_" + util.IntToString(j) + "_" + util.IntToString(k) + "_" + util.IntToString(l);
						tenpai_train_list[str] = sum;
					}
				}
			}
		}
		// boost::unordered_map<std::string, int>::iterator ite;
		// for (ite = tenpai_train_list.begin(); ite != tenpai_train_list.end(); ite++)
		// {
		// 	ofs << ite->first << std::endl;
		// }
		tenpai_train_list.clear();
		ifstream ifs("source/tenpai_pre_tag.txt");
		vector<string> result;
		int j = 1;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			//cout<<result[0]<<endl;
			tenpai_train_list[result[0]] = j;
			j++;
		}
		return sum;
	}
	void tenpai_pre_feature(int pl, int mirugawa, player p[], field *f, int sutehai[], vector<string> *feature, vector<double> *feature_val)
	{
		if (p[pl].ri_ti_flag >= 1) { feature->push_back("r" + util.IntToString(1)); feature_val->push_back(1); return; }
		else { feature->push_back("r" + util.IntToString(0)); feature_val->push_back(1); }
		{
			int i = p[pl].sutehai.size();
			if (i >= 18) { i = 18; }
			feature->push_back("f" + util.IntToString(p[pl].fu_ro_suu) + "sute" + util.IntToString(i)); feature_val->push_back(1);
		}
		for (int i = 0; i<p[pl].fu_ro_suu; i++)
		{
			int j = p[pl].fu_ro_sute[i];
			if (j >= 18) { j = 18; }
			feature->push_back("f" + util.IntToString(i + 1) + "junme" + util.IntToString(j)); feature_val->push_back(1);
		}

		{
			int j = 0;
			for (int i = 0; i<(signed)p[pl].sutehai.size(); i++)
			{
				if (p[pl].tedasi[i] == 1) { j++; }
			}
			if (j >= 18) { j = 18; }
			feature->push_back("f" + util.IntToString(p[pl].fu_ro_suu) + "all_tds" + util.IntToString(j)); feature_val->push_back(1);
		}
		{
			int j = 0;
			int k = 0;
			int last = -1;
			if (p[pl].fu_ro_suu>0) { k = (signed)p[pl].fu_ro_sute[p[pl].fu_ro_suu - 1]; }
			// cout<<"k"<<k<<endl;
            if( k >= 0) {
                for (int i = k; i<(signed)p[pl].sutehai.size(); i++)
                {
                    if (p[pl].tedasi[i] == 1) {
                        j++;
                        last = p[pl].sutehai[k];
                    }
                }
            }
			if (j >= 18) { j = 18; }
			feature->push_back("f" + util.IntToString(p[pl].fu_ro_suu) + "tds" + util.IntToString(j)); feature_val->push_back(1);
			if (last != -1)
			{
				feature->push_back("f" + util.IntToString(p[pl].fu_ro_suu) + "last_tds" + util.IntToString(last)); feature_val->push_back(1);
			}
		}

		for (int i = 0; i<p[pl].fu_ro_suu; i++)
		{
            if(p[pl].fu_ro_sute[i] > -1) { // TODO おかしい
                feature->push_back("f_k" + util.IntToString(p[pl].fu_ro[i]) + "tds" + util.IntToString(p[pl].sutehai[p[pl].fu_ro_sute[i]])); feature_val->push_back(1);
            } else {
                cout << "おかしい fu_ro_sute" << endl;
                cout << "i" << i << endl;
                cout << "pl" << pl << endl;
                cout << "p[pl].fu_ro_suu" << p[pl].fu_ro_suu << endl;
                cout << "p[pl].fu_ro[i]" << p[pl].fu_ro[i] << endl;
                cout << "p[pl].sutehai[0]" << p[pl].sutehai[0] << endl;
                p[pl].fu_ro_suu = 0;
            }
		}
		for (int i = 0; i<5; i++)
		{
			if (f->dora[i] == -1) { break; }
			for (int j = 0; j<(signed)p[pl].sutehai.size(); j++)
			{
				if (f->dora[i] == util.get_kind_of_dora(util.trance37to34(p[pl].sutehai[j])))
				{
					feature->push_back("d" + util.IntToString(f->dora[i])); feature_val->push_back(1);
				}
			}
		}
		for (int j = 0; j<(signed)p[pl].sutehai.size(); j++)
		{
			if (p[pl].sutehai[j]<30 && p[pl].sutehai[j] % 10 == 5)
			{
				feature->push_back("aka"); feature_val->push_back(1);
			}
		}
		{
			int k = 3;
			for (int i = 0; i<p[pl].fu_ro_suu; i++)
			{
				if (k == 3 && p[pl].fu_ro[i] % 34<27) { k = (p[pl].fu_ro[i] % 34) / 9; }
				else if (k != 3 && p[pl].fu_ro[i] % 34<27 &&
					k != (p[pl].fu_ro[i] % 34) / 9) {
					k = -1;
				}
			}

			if (k >= 0 && k <= 2)
			{
				int flag = 0;
				int s_i = 0;
				int junme = p[pl].sutehai.size();
				if (junme >= 18) { junme = 18; }
				for (int j = 0; j<(signed)p[pl].sutehai.size(); j++)
				{
					if (p[pl].sutehai[j] % 10 == k && p[pl].tedasi[j] == 1)
					{
						flag = 2;
					}
					else if (flag == 0 && p[pl].sutehai[j] >= 30 && p[pl].tedasi[j] == 1)
					{
						flag = 1;
					}
					else if (p[pl].sutehai[j] <= 30 && p[pl].sutehai[j] % 10 != k && p[pl].tedasi[j] == 1)
					{
						s_i = flag;
					}
				}
				feature->push_back("some_f" + util.IntToString(p[pl].fu_ro_suu) + "_" + util.IntToString(s_i) + "_" + util.IntToString(flag) + "_" + util.IntToString(junme));
				feature_val->push_back(1);
			}
		}

		{
			int saisho = -1;
			for (int j = 0; j<(signed)p[pl].sutehai.size(); j++)
			{
				if (saisho == -1 && p[pl].tedasi[j] == 1) { saisho = p[pl].sutehai[j]; }
				else if (p[pl].tedasi[j] == 1)
				{
					feature->push_back("sute" + util.IntToString(saisho) + "_" + util.IntToString(p[pl].sutehai[j])); feature_val->push_back(1);
					saisho = p[pl].sutehai[j];
				}
			}
		}
	}
	std::vector<double> cal_tenpai_pro(std::vector<double> tenpai_pro, double **tenpai_wei, int pl, player tatya[], field *game_field, int sutehai[])
	{
		for (int k = 1; k<4; k++)
		{
			double temp[2] = { 0.0 };
			vector<string> te_feature;
			vector<double> te_feature_val;
			tenpai_pre_feature(k, pl, tatya, game_field, sutehai, &te_feature, &te_feature_val);
			for (int cl = 0; cl<2; cl++)
			{
				for (int l = 0; l<(signed)te_feature.size(); l++)
				{
					
                    int t__ = tenpai_pre_list[te_feature[l]] - 1;
                    if(t__ >= 0) {
                        temp[cl] += tenpai_wei[cl][t__] * te_feature_val[l];
                    } else {
#ifndef RELEASE_BUILD
                        cout << "なんで？" << endl;;
                        cout<<t__ << " " << te_feature[l]<<" "<< endl;
                        cout << tenpai_pre_list[te_feature[l]]-1<<" "<< endl;
                        cout << tenpai_wei[cl] << endl;
                        //[tenpai_pre_list[te_feature[l]]-1]<<endl;
#endif
                    }
                        
				}
			}
			if (temp[0]<temp[1])
			{
				temp[0] -= temp[1]; temp[1] = 0.0;
			}
			else
			{
				temp[1] -= temp[0]; temp[0] = 0.0;
			}
			double val = 0.0;
			for (int cl = 0; cl<2; cl++)
			{
				val += exp(temp[cl]);
			}

			tenpai_pro[k - 1] = exp(temp[1]) / val;
		}
		return tenpai_pro;
	}

	//matiyomi
	boost::unordered_map<std::string, int> matiyomi_list;
	//double **matiyomi_wei;
	int init_mati_weight()
	{
		string str;
		int sum = 0;
		// std::ofstream ofs("/data/local/mizukami/matiyomi/matiyomi_tag.txt");
		for (int i = 0; i<34; i++)
		{
			for (int j = 0; j <= 4; j++)
			{
				sum++;
				str = "H" + util.IntToString(i) + "_" + util.IntToString(j);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 0; i<34; i++)
		{
			for (int j = -1; j<3; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					sum++;
					str = "co" + util.IntToString(k) + "_" + util.IntToString(j) + "_" + util.IntToString(i);
					matiyomi_list[str] = sum;
				}
			}
		}

		for (int i = 0; i<34; i++)
		{
			sum++;
			str = "G" + util.IntToString(i);
			matiyomi_list[str] = sum;
		}
		for (int i = 0; i<34; i++)
		{
			for (int j = 0; j<3; j++)
			{
				sum++;
				str = "tgwr_" + util.IntToString(i) + "_" + util.IntToString(j);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 0; i<34; i++)
		{
			sum++;
			str = "D" + util.IntToString(i);
			matiyomi_list[str] = sum;
		}
		for (int i = 0; i <= 17; i++)
		{
			for (int j = 0; j<37; j++)
			{
				sum++;
				str = "jd" + util.IntToString(i) + "_" + util.IntToString(j);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 0; i<37; i++)
		{
			sum++;
			str = "R" + util.IntToString(i);
			matiyomi_list[str] = sum;
		}

		for (int i = 0; i<34; i++)
		{
			for (int j = 0; j<136; j++)
			{
				sum++;
				str = "f" + util.IntToString(j) + "_" + util.IntToString(i);
				matiyomi_list[str] = sum;
			}
		}

		for (int j = 0; j<136; j++)
		{
			sum++;
			str = "f1_" + util.IntToString(j);
			matiyomi_list[str] = sum;
		}
		for (int i = 0; i<136; i++)
		{
			for (int j = 0; j<136; j++)
			{
				sum++;
				str = "f2_" + util.IntToString(j) + "_" + util.IntToString(i);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 0; i<37; i++)
		{
			for (int j = 0; j<37; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					for (int l = 0; l <= 1; l++)
					{
						sum++;
						str = "sute" + util.IntToString(k) + util.IntToString(l) + "_" + util.IntToString(i) + "_" + util.IntToString(j);
						matiyomi_list[str] = sum;
					}
				}
			}
		}

		for (int kk = 0; kk <= 2; kk++)
		{
			for (int k = kk; k <= 2; k++)
			{
				for (int i = 0; i <= 4; i++)
				{
					for (int j = 0; j <= 2; j++)
					{
						for (int l = 0; l <= 17; l++)
						{
							sum++;
							str = "some" + util.IntToString(j) + "_" + util.IntToString(i) + "_" + util.IntToString(kk) + "_" + util.IntToString(k) + "_" + util.IntToString(l);
							matiyomi_list[str] = sum;
						}
					}
				}
			}
		}

		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				sum++;
				str = "ta_f" + util.IntToString(j) + "_" + util.IntToString(i);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				sum++;
				str = "tyanta_f" + util.IntToString(j) + "_" + util.IntToString(i);
				matiyomi_list[str] = sum;
			}
		}
		for (int i = 27; i <= 30; i++)
		{
			sum++;
			str = "ji" + util.IntToString(i);
			matiyomi_list[str] = sum;
		}
		for (int i = 27; i<30; i++)
		{
			sum++;
			str = "ba" + util.IntToString(i);
			matiyomi_list[str] = sum;
		}

		sum++;
		str = "bias";
		matiyomi_list[str] = sum;

		// boost::unordered_map<std::string, int>::iterator ite;
		// for (ite = matiyomi_list.begin(); ite != matiyomi_list.end(); ite++)
		// {
		// 	ofs << ite->first << std::endl;
		// }
		matiyomi_list.clear();
		ifstream ifs("source/matiyomi_tag.txt");
		vector<string> result;
		int j = 1;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			//cout<<result[0]<<endl;
			matiyomi_list[result[0]] = j;
			j++;
		}
		return sum;
	}
	void combi(int mieteruhai[], int anzen, int genbutu[], double kumiawase[])
	{
		for (int i = 0; i<34; i++)
		{
			kumiawase[i] = 0.0;
		}

		int kumi_all = 1;
		int kumi[34] = { 0 };

		for (int i = 0; i<34; i++)
		{
			if (mieteruhai[i] == 0 && genbutu[i]>anzen)
			{
				kumi_all += 3;
				kumi[i] += 3;
			}
			else if (mieteruhai[i] == 1 && genbutu[i]>anzen)
			{
				kumi_all += 1;
				kumi[i] += 1;
			}
		}
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<9; j++)
			{
				if ((j<6 && genbutu[i * 9 + j + 3]>anzen) || j == 6)
				{
					kumi_all += (4 - mieteruhai[i * 9 + j + 1])*(4 - mieteruhai[i * 9 + j + 2]);
					kumi[i * 9 + j] += (4 - mieteruhai[i * 9 + j + 1])*(4 - mieteruhai[i * 9 + j + 2]);
				}
				if ((j>2 && genbutu[i * 9 + j - 3]>anzen) || j == 2)
				{
					kumi_all += (4 - mieteruhai[i * 9 + j - 1])*(4 - mieteruhai[i * 9 + j - 2]);
					kumi[i * 9 + j] += (4 - mieteruhai[i * 9 + j - 1])*(4 - mieteruhai[i * 9 + j - 2]);
				}
				if (j != 0 && j != 8)
				{
					kumi_all += (4 - mieteruhai[i * 9 + j - 1])*(4 - mieteruhai[i * 9 + j + 1]);
					kumi[i * 9 + j] += (4 - mieteruhai[i * 9 + j - 1])*(4 - mieteruhai[i * 9 + j + 1]);
				}
			}
		}
		for (int i = 0; i<34; i++)
		{
			kumiawase[i] = (double)kumi[i] / (double)kumi_all;
		}
	}
	void matiyomi_feature(int mieteruhai[], int pl, int mirugawa, player tatya[], field *game_field, vector<string> *feature, vector<double> *feature_val)
	{
		int k = 0;

		for (int i = 0; i<34; i++)
		{
			feature->push_back("H" + util.IntToString(i) + "_" + util.IntToString(mieteruhai[i]));
			feature_val->push_back(1);
		}

		{
			int genbutu[34] = { 0 };
			for (int i = 0; i<34; i++)
			{
				genbutu[i] = tatya[pl].tegawari[i];
			}
			for (int i = 0; i<34; i++)
			{
				if (tatya[pl].genbutu[i] == 1) { genbutu[i] = 0; }
			}
			double kumiawase[4][34] = { { 0.0 } };
			for (int j = -1; j<3; j++)
			{
				combi(mieteruhai, j, genbutu, kumiawase[j + 1]);
			}
			for (int i = 0; i<34; i++)
			{
				for (int j = -1; j<3; j++)
				{
					feature->push_back("co" + util.IntToString(tatya[pl].ri_ti_flag) + "_" + util.IntToString(j) + "_" + util.IntToString(i));
					if (j == -1) { feature_val->push_back(kumiawase[0][i]); }
					else { feature_val->push_back(kumiawase[j][i] - kumiawase[j + 1][i]); }
				}
			}
		}
		for (int i = 0; i<34; i++)
		{
			if (tatya[pl].tegawari[i] == 0 || tatya[pl].genbutu[i] >= 1)
			{
				feature->push_back("G" + util.IntToString(i));
				feature_val->push_back(1);
			}
		}

		{
			int genbutu[34] = { 0 };
			for (int i = 0; i<34; i++)
			{
				genbutu[i] = tatya[pl].tegawari[i];
			}
			for (int i = 0; i<34; i++)
			{
				if (tatya[pl].genbutu[i] == 1)
				{
					genbutu[i] = 0;
				}
			}
			for (int i = 0; i<34; i++)
			{
				if (genbutu[i] <= 2 && genbutu[i]>0 && tatya[pl].ri_ti_flag != 1)
				{
					feature->push_back("tgwr_" + util.IntToString(i) + "_" + util.IntToString(genbutu[i]));
					feature_val->push_back(1);
				}
			}
		}
		for (int i = 0; i<5; i++)
		{
			if (game_field->dora[i] != -1)
			{
				feature->push_back("D" + util.IntToString(util.get_kind_of_dora(game_field->dora[i])));
				feature_val->push_back(1);
			}
		}
		for (int i = 0; i<(signed)tatya[pl].sutehai.size(); i++)
		{
			if (i <= tatya[pl].ri_ti_sute || tatya[pl].ri_ti_sute == -1)
			{
				feature->push_back("jd" + util.IntToString(i) + "_" + util.IntToString(tatya[pl].sutehai[i]));
				feature_val->push_back(1);
			}
			if (i == 17) { break; }
		}

		if (tatya[pl].ri_ti_sute != -1)
		{
			feature->push_back("R" + util.IntToString(tatya[pl].sutehai[tatya[pl].ri_ti_sute]));
			feature_val->push_back(1);
		}
		for (int i = 0; i<4; i++)
		{
			if (tatya[pl].fu_ro[i] != -1
                && pl < 4
                && tatya[pl].fu_ro_sute[i] != -1)
			{
				feature->push_back("f" + util.IntToString(tatya[pl].fu_ro[i]) + "_" + util.IntToString(util.trance37to34(tatya[pl].sutehai[tatya[pl].fu_ro_sute[i]])));
				feature_val->push_back(1);
			}
		}
		for (int i = 0; i<tatya[pl].fu_ro_suu; i++)
		{
			feature->push_back("f1_" + util.IntToString(tatya[pl].fu_ro[i]));
			feature_val->push_back(1);
		}
		for (int i = 0; i<tatya[pl].fu_ro_suu - 1; i++)
		{
			for (int j = i + 1; j<tatya[pl].fu_ro_suu; j++)
			{
				feature->push_back("f2_" + util.IntToString(tatya[pl].fu_ro[i]) + "_" + util.IntToString(tatya[pl].fu_ro[j]));
				feature_val->push_back(1);
			}
		}


		for (int i = 0; i<(signed)tatya[pl].sutehai.size() - 1; i++)
		{
			for (int j = i + 1; j<(signed)tatya[pl].sutehai.size(); j++)
			{
				if (j <= tatya[pl].ri_ti_sute || tatya[pl].ri_ti_sute == -1)
				{
					feature->push_back("sute" + util.IntToString(tatya[pl].tedasi[i]) + util.IntToString(tatya[pl].tedasi[j]) + "_" + util.IntToString(tatya[pl].sutehai[i]) + "_" + util.IntToString(tatya[pl].sutehai[j]));
					//feature->push_back("sute"+util.IntToString(tatya[pl].sutehai[i])+"_"+util.IntToString(tatya[pl].sutehai[j]));
					feature_val->push_back(1);
				}
			}
		}

		k = 1;
		for (int i = 0; i<tatya[pl].fu_ro_suu; i++)
		{
			if (!((tatya[pl].fu_ro[i]<34 && tatya[pl].fu_ro[i] % 9 != 0 && tatya[pl].fu_ro[i] % 9 != 6) ||
				(tatya[pl].fu_ro[i] >= 34 && tatya[pl].fu_ro[i] % 34<27 && (tatya[pl].fu_ro[i] % 34) % 9 != 0 && (tatya[pl].fu_ro[i] % 34) % 9 != 8)))
			{
				k = 0;
			}
		}
		feature->push_back("ta_f" + util.IntToString(k) + "_" + util.IntToString(tatya[pl].fu_ro_suu));
		feature_val->push_back(1);

		k = 1;
		for (int i = 0; i<tatya[pl].fu_ro_suu; i++)
		{
			if (!((tatya[pl].fu_ro[i]<34 && tatya[pl].fu_ro[i] % 9 != 0 && tatya[pl].fu_ro[i] % 9 != 6) ||
				(tatya[pl].fu_ro[i] >= 34 && tatya[pl].fu_ro[i] % 34<27 && (tatya[pl].fu_ro[i] % 34) % 9 != 0 && (tatya[pl].fu_ro[i] % 34) % 9 != 8)))
			{
				k = 0;
			}
		}
		feature->push_back("ta_f" + util.IntToString(k) + "_" + util.IntToString(tatya[pl].fu_ro_suu));
		feature_val->push_back(1);

		k = 1;
		for (int i = 0; i<tatya[pl].fu_ro_suu; i++)
		{
			if (!((tatya[pl].fu_ro[i]<34 && (tatya[pl].fu_ro[i] % 9 == 0 || tatya[pl].fu_ro[i] % 9 == 6)) ||
				(tatya[pl].fu_ro[i] >= 34 && ((tatya[pl].fu_ro[i] % 34) % 9 == 0 || (tatya[pl].fu_ro[i] % 34) % 9 == 8))))
			{
				k = 0;
			}
		}
		feature->push_back("tyanta_f" + util.IntToString(k) + "_" + util.IntToString(tatya[pl].fu_ro_suu));
		feature_val->push_back(1);

		feature->push_back("ba" + util.IntToString(game_field->bafuu));
		feature_val->push_back(1);

		feature->push_back("ji" + util.IntToString(tatya[pl].jifuu));
		feature_val->push_back(1);
		k = 3;
		for (int i = 0; i<tatya[pl].fu_ro_suu; i++)
		{
			if (k == 3 && tatya[pl].fu_ro[i] % 34<27) { k = (tatya[pl].fu_ro[i] % 34) / 9; }
			else if (k != 3 && tatya[pl].fu_ro[i] % 34<27 &&
				k != (tatya[pl].fu_ro[i] % 34) / 9) {
				k = -1;
			}
		}

		if (k >= 0 && k <= 2)
		{
			int flag = 0;
			int s_i = 0;
			int junme = tatya[pl].sutehai.size();
			if (junme >= 18) { junme = 17; }
			for (int j = 0; j<(signed)tatya[pl].sutehai.size(); j++)
			{
				if (tatya[pl].sutehai[j] % 10 == k && tatya[pl].tedasi[j] == 1)
				{
					flag = 2;
				}
				else if (flag == 0 && tatya[pl].sutehai[j] >= 30 && tatya[pl].tedasi[j] == 1)
				{
					flag = 1;
				}
				else if (tatya[pl].sutehai[j]<30 && tatya[pl].sutehai[j] % 10 != k && tatya[pl].tedasi[j] == 1)
				{
					s_i = flag;
				}
			}
			feature->push_back("some" + util.IntToString(k) + "_" + util.IntToString(tatya[pl].fu_ro_suu) + "_" + util.IntToString(s_i) + "_" + util.IntToString(flag) + "_" + util.IntToString(junme));
			feature_val->push_back(1);
		}

		feature->push_back("bias");
		feature_val->push_back(1);

	}
	boost::unordered_map<string, double> cal_matiyomi_pro(double **matiyomi_wei, int pl, player tatya[], field *game_field, int mieteruhai[])
	{
		boost::unordered_map<string, int> matiyomi_list_temp = matiyomi_list;
		static boost::unordered_map<string, double> matiyomi_pro;
		matiyomi_pro.clear();

		//ofstream ofsaa("masdfg.txt");
		for (int k = 1; k<4; k++)
		{
			vector<string> mati_feature;
			vector<double> mati_feature_val;
			matiyomi_feature(mieteruhai, k, pl, tatya, game_field, &mati_feature, &mati_feature_val);

			for (int l = 0; l<34; l++)
			{
                //std::cout << l << "@" << mieteruhai[l] << ",";
                
				for (int f_len = 0; f_len<(signed)mati_feature.size(); f_len++)
				{
					//  if(k==2&&tatya[2].ri_ti_flag==1&&l==26){ofsaa<<mati_feature[f_len]<<" "<<matiyomi_wei[l][matiyomi_list[mati_feature[f_len]]-1]<<" "<<mati_feature_val[f_len]<<endl;}
                    

                    
                    double tmp1 = mati_feature_val[f_len];
                    double tmp2_0 = matiyomi_list_temp[mati_feature[f_len]];
                    if(tmp2_0 <= 0){
                        std::cout << "er bkuc cal_matiyomi_prob" << std::endl;
                        std::cout << matiyomi_list_temp[mati_feature[f_len]] << std::endl;
                        std::cout << matiyomi_wei[l][matiyomi_list_temp[mati_feature[f_len]]] << std::endl;
                        std::cout << util.IntToString(k) << std::endl;
                        std::cout << util.IntToString(l) << std::endl;
                        std::cout << mati_feature[f_len] << std::endl;
                        continue;
                    }
                    double tmp2 = matiyomi_wei[l][(int)tmp2_0 - 1];
                    double tmp = tmp1 * tmp2;
                    
                    matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)] += tmp;
						
				}
				

				double tmp = matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)];
				if (matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)]<0.0) { matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)] = exp(2.0*tmp) / (exp(0.0) + exp(2.0*tmp)); }
				else { matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)] = exp(0.0) / (exp(0.0) + exp(-2.0*tmp)); }
                
                if(tatya[k].genbutu[l]==1) {
                    //std::cout << k << "_" << l << " genbutu" << std::endl;
                    matiyomi_pro[util.IntToString(k) + "_" + util.IntToString(l)] = 0;
                }
                

				//if(asdfgh==1&&k==2&&l==13){cout<<matiyomi_pro[util.IntToString(k)+"_"+util.IntToString(l)]<<endl;}
			}
			//if(k==2&&tatya[2].ri_ti_flag==1){while(1){}}
		}
		Util::writeFileToLogTxt(std::string("cal_matiyomi_pro end"));
		return matiyomi_pro;
	}

	//tokuten_pre
	boost::unordered_map<std::string, int> tokuten_pre_list;
	boost::unordered_map<std::string, int> tokuten_train_list;
	//extern double **tokuten_wei;
	int init_tokuten_pre_weight()
	{
		string str;
		int sum = 0;
		// std::ofstream ofs("/data/local/mizukami/tokuten_pre/tokuten_pre_tag.txt");

		for (int i = 0; i<2; i++)
		{
			for (int rt = 0; rt <= 1; rt++) {
				for (int riich = 0; riich <= 1; riich++) {
					sum++;
					str = "oya" + util.IntToString(i) + "rt" + util.IntToString(rt) + "riich" + util.IntToString(riich);
					tokuten_train_list[str] = sum;
				}
			}
		}
		for (int i = -1; i<2; i++)//-1fu_ro 0dama 1ri-ti
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int k = 0; k <= 7; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "men" + util.IntToString(i) + "kaku" + util.IntToString(j) + "mie" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}

		for (int i = 0; i<136; i++)
		{
			for (int kk = 0; kk <= 6; kk++)
			{
				for (int k = 0; k <= 7; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "f" + util.IntToString(i) + "kaku" + util.IntToString(kk) + "mie" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}
		for (int i = 0; i<136; i++)
		{
			for (int kk = 0; kk<136; kk++)
			{
				for (int rt = 0; rt <= 1; rt++)
				{
					sum++;
					str = "f" + util.IntToString(i) + "f" + util.IntToString(kk) + "rt" + util.IntToString(rt);
					tokuten_train_list[str] = sum;
				}
			}
		}

		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int k = 0; k <= 7; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "f" + util.IntToString(i) + "kaku" + util.IntToString(j) + "md" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}
		for (int i = -1; i <= 1; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "r" + util.IntToString(i) + "sj" + util.IntToString(j) + "hj" + util.IntToString(k) + "rt" + util.IntToString(rt);//suji 19hai=hj ka
						tokuten_train_list[str] = sum;
					}
				}
			}
		}

		for (int i = 0; i <= 7; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int rt = 0; rt <= 1; rt++)
				{
					sum++;
					str = "r_md" + util.IntToString(i) + "kaku" + util.IntToString(j) + "rt" + util.IntToString(rt);
					tokuten_train_list[str] = sum;
				}
			}
		}

		for (int i = 1; i <= 2; i++)
		{
			for (int rt = 0; rt <= 1; rt++)
			{
				sum++;
				str = "ota_yaku" + util.IntToString(i) + "rt" + util.IntToString(rt);
				tokuten_train_list[str] = sum;
			}
		}
		for (int i = 0; i <= 4; i++)
		{
			for (int rt = 0; rt <= 1; rt++)
			{
				sum++;
				str = "dora_tonari" + util.IntToString(i) + "rt" + util.IntToString(rt);
				tokuten_train_list[str] = sum;
			}
		}

		for (int i = 0; i <= 1; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				for (int k = 0; k <= 7; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "tan" + util.IntToString(i) + "d" + util.IntToString(j) + "mie" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}

		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "ho_f" + util.IntToString(i) + "ka" + util.IntToString(j) + "d" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}

		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					for (int rt = 0; rt <= 1; rt++)
					{
						sum++;
						str = "ti_f" + util.IntToString(i) + "ka" + util.IntToString(j) + "d" + util.IntToString(k) + "rt" + util.IntToString(rt);
						tokuten_train_list[str] = sum;
					}
				}
			}
		}


		for (int i = 0; i <= 4; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				for (int rt = 0; rt <= 1; rt++)
				{
					sum++;
					str = "toi_f" + util.IntToString(i) + "ka" + util.IntToString(j) + "rt" + util.IntToString(rt);
					tokuten_train_list[str] = sum;
				}
			}
		}
		for (int i = 1; i <= 2; i++)
		{
			for (int rt = 0; rt <= 1; rt++)
			{
				sum++;
				str = "sangen" + util.IntToString(i) + "rt" + util.IntToString(rt);
				tokuten_train_list[str] = sum;
			}
		}
		for (int i = 2; i <= 3; i++)
		{
			for (int rt = 0; rt <= 1; rt++)
			{
				sum++;
				str = "susi" + util.IntToString(i) + "rt" + util.IntToString(rt);
				tokuten_train_list[str] = sum;
			}
		}
		// boost::unordered_map<std::string, int>::iterator ite;
		// for (ite = tokuten_train_list.begin(); ite != tokuten_train_list.end(); ite++)
		// {
		// 	ofs << ite->first << std::endl;
		// }
		tokuten_train_list.clear();
		ifstream ifs("source/tokuten_pre_tag.txt");
		vector<string> result;
		int j = 1;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			//cout<<result[0]<<endl;
			tokuten_train_list[result[0]] = j;
			j++;
		}
		return sum;
	}
	void tokuten_pre_feature(int pl, int mirugawa, int ron_tsumo, int agarihai, player p[], field *f, int mieteruhai[], vector<string> *feature, vector<double> *feature_val)
	{
		int miedora = mieteru_dora(mieteruhai, p, f);
		miedora += get_number_of_aka(p[mirugawa].aka_moti);
		if (miedora >= 7) { miedora = 7; }
		feature->push_back("oya" + util.IntToString(p[pl].jifuu == 27) + "rt" + util.IntToString(ron_tsumo) + "riich" + util.IntToString(p[pl].ri_ti_flag>0)); feature_val->push_back(1);
		{
			int kaku = p[pl].ri_ti_flag;
			for (int i = 0; i<4; i++)
			{
				if (p[pl].fu_ro[i] == -1) { break; }
				if (p[pl].fu_ro[i] % 34 == f->bafuu) { kaku++; }
				if (p[pl].fu_ro[i] % 34 == p[pl].jifuu) { kaku++; }
				if (p[pl].fu_ro[i] % 34 >= 31) { kaku++; }
			}
			for (int i = 0; i<5; i++)
			{
				if (f->dora[i] == -1) { break; }
				if (util.get_kind_of_dora(f->dora[i]) == agarihai) { kaku++; }
			}
			kaku += sarasi_dora(p[pl], f);
			kaku += p[pl].ippatu;
			kaku += (cut_aka(p[mirugawa].hai, agarihai, p[mirugawa].aka_moti)>0);
			if (kaku >= 6) { kaku = 6; }

			{
				int j = 0;
				if (p[pl].ri_ti_flag>0) { j = 1; }
				for (int i = 0; i<4; i++)
				{
					if (p[pl].fu_ro[i] >= 0 && p[pl].fu_ro[i]<102) { j = -1; }
				}
				feature->push_back("men" + util.IntToString(j) + "kaku" + util.IntToString(kaku) + "mie" + util.IntToString(miedora) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);

				for (int i = 0; i<4; i++)
				{
					if (p[pl].fu_ro[i] >= 0) {
						feature->push_back("f" + util.IntToString(p[pl].fu_ro[i]) + "kaku" + util.IntToString(kaku) + "mie" + util.IntToString(miedora) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
					}
				}
				for (int i = 0; i<4; i++)
				{
					for (int ii = i; ii<4; ii++)
					{
						if (p[pl].fu_ro[i] >= 0 && p[pl].fu_ro[ii] >= 0)
						{
							feature->push_back("f" + util.IntToString(p[pl].fu_ro[i]) + "f" + util.IntToString(p[pl].fu_ro[ii]) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
						}
					}
				}


				int genbutu[34] = { 0 };
				for (int i = 0; i<34; i++) { genbutu[i] = p[pl].tegawari[i]; }
				for (int i = 0; i<34; i++) { if (p[pl].genbutu[i] == 1) { genbutu[i] = 0; } }
				for (int i = 0; i<34; i++)
				{
					if (genbutu[i] == 0) { genbutu[i] = 1; }
					else { genbutu[i] = 0; }
				}
				if (agarihai >= 0)
				{
					int k = 0;
					int l = 0;
					if (agarihai<27)
					{
						if (agarihai % 9 <= 2 && (genbutu[agarihai + 3] == 1 || mieteruhai[agarihai + 2] == 4 || mieteruhai[agarihai + 1] == 4)) { k = 1; }
						else if (agarihai % 9 >= 6 && (genbutu[agarihai - 3] == 1 || mieteruhai[agarihai - 2] == 4 || mieteruhai[agarihai - 1] == 4)) { k = 1; }
						else if (agarihai % 9 >= 3 && agarihai % 9 <= 5 &&
							((genbutu[agarihai + 3] == 1 && genbutu[agarihai - 3] == 1) ||
								((mieteruhai[agarihai + 2] == 4 || mieteruhai[agarihai + 1] == 4) && (mieteruhai[agarihai - 2] == 4 || mieteruhai[agarihai - 1] == 4)))) {
							k = 1;
						}

						if (agarihai % 9 == 0 || agarihai % 9 == 8) { l = 1; }
					}
					else { k = 1; l = 1; }
					feature->push_back("r" + util.IntToString(j) + "sj" + util.IntToString(k) + "hj" + util.IntToString(l) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
				}
			}

			if (p[pl].ri_ti_flag>0) { feature->push_back("r_md" + util.IntToString(miedora) + "kaku" + util.IntToString(kaku) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
			else { feature->push_back("f" + util.IntToString(p[pl].fu_ro_suu) + "kaku" + util.IntToString(kaku) + "md" + util.IntToString(miedora) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }

			int ota = 0;
			if (p[pl].fu_ro_suu>0 && p[pl].fu_ro[0] % 34 >= 27 && !(p[pl].fu_ro[0] % 34 != f->bafuu || p[pl].fu_ro[0] % 34 == p[pl].jifuu || p[pl].fu_ro[0] % 34 >= 31))
			{
				ota = 1;
				for (int i = 1; i<p[pl].fu_ro_suu; i++)
				{
					if ((p[pl].fu_ro[i] % 34 == f->bafuu || p[pl].fu_ro[i] % 34 == p[pl].jifuu || p[pl].fu_ro[i] % 34 >= 31)) { ota = 2; }
				}
				feature->push_back("ota_yaku" + util.IntToString(ota) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
			}

			{
				int j = util.get_kind_of_dora(f->dora[0]);
				if (j == agarihai) { feature->push_back("dora_tonari0rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
				else if (j / 9 != agarihai / 9 || j >= 27) { feature->push_back("dora_tonari4rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
				else if (j + 1 == agarihai || j - 1 == agarihai) { feature->push_back("dora_tonari1rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
				else if (j + 2 == agarihai || j - 2 == agarihai) { feature->push_back("dora_tonari2rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
				else { feature->push_back("dora_tonari3rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }

				int k = 1;
				if (p[pl].fu_ro_suu>0)
				{
					for (int i = 0; i<p[pl].fu_ro_suu; i++)
					{
						if (!((p[pl].fu_ro[i]<34 && p[pl].fu_ro[i] % 9 != 0 && p[pl].fu_ro[i] % 9 != 6) ||
							(p[pl].fu_ro[i] >= 34 && p[pl].fu_ro[i] % 34<27 && (p[pl].fu_ro[i] % 34) % 9 != 0 && (p[pl].fu_ro[i] % 34) % 9 != 8)))
						{
							k = 0;
						}
					}
					if (j % 9 == 0 || j % 9 == 8 || j >= 27) { {feature->push_back("tan" + util.IntToString(k) + "d" + util.IntToString(0) + "mie" + util.IntToString(miedora) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); } }
					else { feature->push_back("tan" + util.IntToString(k) + "d" + util.IntToString(1) + "mie" + util.IntToString(miedora) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1); }
				}
			}

			{
				int k = 3;
				for (int i = 0; i<p[pl].fu_ro_suu; i++)
				{
					if (k == 3 && p[pl].fu_ro[i] % 34<27)
					{
						k = (p[pl].fu_ro[i] % 34) / 9;
					}
					else if (k != 3 && p[pl].fu_ro[i] % 34<27 && k != (p[pl].fu_ro[i] % 34) / 9)
					{
						k = -1;
					}
				}
				if (k != -1)
				{
					int j = util.get_kind_of_dora(f->dora[0]);
					if (k == 3 || k == agarihai / 9 || agarihai >= 27 || agarihai == -1)
					{
						feature->push_back("ho_f" + util.IntToString(p[pl].fu_ro_suu) + "ka" + util.IntToString(kaku) + "d" + util.IntToString(j / 9<3 && j / 9 == k) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
					}
				}
			}

			{
				int k = 4;
				for (int i = 0; i<p[pl].fu_ro_suu; i++)
				{
					if (p[pl].fu_ro[i] % 34 >= 27) { k = -1; }
					if (k == 4) { k = (p[pl].fu_ro[i] % 34) / 9; }
					else if (k != 4 && k != -1 && k != (p[pl].fu_ro[i] % 34) / 9) { k = -1; }
				}
				if (k != -1)
				{
					int j = util.get_kind_of_dora(f->dora[0]);
					if (k == 3 || k == agarihai / 9 || agarihai >= 27 || agarihai == -1)
					{
						feature->push_back("ti_f" + util.IntToString(p[pl].fu_ro_suu) + "ka" + util.IntToString(kaku) + "d" + util.IntToString(j / 9<3 && j / 9 == k) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
					}
				}
			}

			{
				int k = 1;
				for (int i = 0; i<p[pl].fu_ro_suu; i++)
				{
					if (p[pl].fu_ro[i]<34) { k = 0; }
				}
				if (k == 1)
				{
					feature->push_back("toi_f" + util.IntToString(p[pl].fu_ro_suu) + "ka" + util.IntToString(kaku) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
				}
			}

			if (agarihai >= 31)
			{
				int haku = 0, hatu = 0, tyun = 0;
				for (int i = 0; i<p[pl].fu_ro_suu; i++)
				{
					if (p[pl].fu_ro[i] % 34 == 31) { haku = 1; }
					else if (p[pl].fu_ro[i] % 34 == 32) { hatu = 1; }
					else if (p[pl].fu_ro[i] % 34 == 33) { tyun = 1; }
				}
				if ((haku + hatu + tyun >= 1 && haku + hatu + tyun <= 2) && mieteruhai[agarihai] <= 2 &&
					((agarihai == 31 && haku == 0) || (agarihai == 32 && hatu == 0) || (agarihai == 33 && tyun == 0)))
				{
					feature->push_back("sangen" + util.IntToString(haku + hatu + tyun) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
				}
			}
			if (agarihai<31 && agarihai >= 27)
			{
				int t = 0, n = 0, s = 0, pe = 0;
				for (int i = 0; i<p[pl].fu_ro_suu; i++)
				{
					if (p[pl].fu_ro[i] % 34 == 27) { t = 1; }
					else if (p[pl].fu_ro[i] % 34 == 28) { n = 1; }
					else if (p[pl].fu_ro[i] % 34 == 29) { s = 1; }
					else if (p[pl].fu_ro[i] % 34 == 30) { pe = 1; }
				}
				if ((t + n + s + pe >= 2 && t + n + s + pe <= 3) && mieteruhai[agarihai] <= 2 &&
					((agarihai == 27 && t == 0) || (agarihai == 28 && n == 0) || (agarihai == 29 && s == 0) || (agarihai == 30 && pe == 0)))
				{
					feature->push_back("susi" + util.IntToString(t + n + s + pe) + "rt" + util.IntToString(ron_tsumo)); feature_val->push_back(1);
				}
			}
		}
	}
	void cal_tokuten_pro(boost::unordered_map<string, double> &tokuten_pro, double *tokuten_wei, int pl, player tatya[], field *game_field, int mieteruhai[])
	{
		boost::unordered_map<std::string, int> tokuten_pre_list_temp = tokuten_pre_list;



		for (int k = 1; k<4; k++)
		{
			for (int l = -1; l<34; l++)//-1はツモ
			{
				vector<string> tokuten_feature;
				vector<double> tokuten_feature_val;
				tokuten_pre_feature(k, 0, l == -1, l, tatya, game_field, mieteruhai, &tokuten_feature, &tokuten_feature_val);
				tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)] = 0.0;

				//Util::writeFileToLogTxt("there1");
				//char aabb[50];
				//sprintf(aabb,"tokuten_pre_list.size()=%d", tokuten_pre_list.size());
				//Util::writeFileToLogTxt(aabb);

				//auto found = tokuten_pre_list_temp.find(tokuten_feature[0]);
				//if (found != tokuten_pre_list_temp.end()) {
				//	Util::writeFileToLogTxt("there");
				//	Util::writeFileToLogTxt(tokuten_feature[0]);
				//	
				//}
				//else {
				//	// Not there
				//	Util::writeFileToLogTxt("Not there");
				//	Util::writeFileToLogTxt(tokuten_feature[0]);
				//}

				for (int f_len = 0; f_len<(signed)tokuten_feature.size(); f_len++)
				{
					// todo otiru
					tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)] += tokuten_wei[tokuten_pre_list_temp[tokuten_feature[f_len]] - 1] * tokuten_feature_val[f_len];
					
				}
				tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)] = exp(tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)]);
				if (tatya[k].jifuu == 27) { tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)] *= 1.5; }
				else if (l == -1 && tatya[0].jifuu == 27) { tokuten_pro[util.IntToString(k) + "_" + util.IntToString(l)] *= 2; }
			}
		}
	}

	void init_engine()
	{
		int sum[3] = { 0 };
		std::string execDir = Util::getExecutableDir();
		string filename = "ShantenTableNew.txt";
		std::string fullPath = execDir + "/" + filename;
		util.init(fullPath);
		ifstream ifs(execDir + "/source/train_naki_int.txt");

		if (!ifs) { cout << "train_naki_intは開けません" << endl; while (1) {} }

		Util::writeFileToLogTxt(std::string("if (!ifs) { cout"));

		string str;
		vector<string> result;
		int j = 0;
		//op_list = (int*)malloc(sizeof(int)*(0x0fffffff));
		//memset(op_list, 0, sizeof(op_list));
		//if (op_list == NULL) { exit(1); }

		boost::unordered_map<std::string, int> op_list_temp;

		Util::writeFileToLogTxt(std::string("memset(op_list, 0, sizeof(op_list));"));

		while (getline(ifs, str))
		{
			//Util::writeFileToLogTxt("333");
			boost::split(result, str, boost::is_space());
			//Util::writeFileToLogTxt(util.IntToString(util.toInt(result[1])));
			//op_list_temp["abv"] = 1;
			//Util::writeFileToLogTxt("444");

			op_list_temp[util.IntToString(util.toInt(result[1]))] = (int)(util.string2double(result[2]) * 1000);
		}
		op_list = op_list_temp;
		Util::writeFileToLogTxt(std::string("ifs.close();"));

		ifs.close();
		ifs.open(execDir + "/source/tenpai_tag.txt");
		if (!ifs) { cout << "tenpai_tagは開けません" << endl; while (1) {} }

		j = 0;

		Util::writeFileToLogTxt(std::string("if (!ifs) { cout << tenpai_tagは開けません << endl; while (1) {} }"));

		boost::unordered_map<std::string, int> tenpai_pre_list_tmp;
		
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			j++;
			//Util::writeFileToLogTxt(result[0]);
			tenpai_pre_list_tmp[result[0]] = j;
			//Util::writeFileToLogTxt("4");
			
		}
		tenpai_pre_list = tenpai_pre_list_tmp;



		Util::writeFileToLogTxt(std::string("sum[0] = j;"));

		sum[0] = j;

		ifs.close();
		ifs.open(execDir + "/source/matiyomi_tag.txt");
		if (!ifs) { cout << "matiyomi_tagは開けません" << endl; while (1) {} }

		j = 0;

		Util::writeFileToLogTxt(std::string("if (!ifs) { cout << matiyomi_tagは開けません << endl; while (1) {} }"));

		boost::unordered_map<std::string, int> matiyomi_list_temp;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			j++;
			matiyomi_list_temp[result[0]] = j;
		}
		matiyomi_list = matiyomi_list_temp;

		sum[1] = j;
		ifs.close();
		ifs.open(execDir + "/source/tokuten_pre_tag.txt");
		if (!ifs) { cout << "tokuten_pre_tagは開けません" << endl; while (1) {} }

		Util::writeFileToLogTxt(std::string("if (!ifs) { cout << tokuten_pre_tagは開けません << endl; while (1) {} }"));

		j = 0;

		boost::unordered_map<std::string, int> tokuten_pre_list_temp;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			j++;
			tokuten_pre_list_temp[result[0]] = j;
		}
		tokuten_pre_list = tokuten_pre_list_temp;

		sum[2] = j;
		ifs.close();
		ifs.open(execDir + "/source/uke_result.txt");
		if (!ifs) { cout << "uke_resultは開けません" << endl; while (1) {
			Util::writeFileToLogTxt(std::string("uke_resultは開けません"));
		} }
		/*
		util.uke_list = (long long*)malloc(sizeof(long long)*(0x07ffffff));
		if (util.uke_list == NULL) { cout << "メモリ足りないbbb" << endl; while (1) {
			Util::writeFileToLogTxt("メモリ足りないbbb");
			exit(1);
		}exit(1); }
		*/
		//memset(util.uke_list, 0, sizeof(util.uke_list));
		Util::writeFileToLogTxt(std::string("memset(util.uke_list, 0, sizeof(util.uke_list));"));

		//int i=0;
		boost::unordered_map<std::string, int> uke_list_temp;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			uke_list_temp[util.IntToString((util.toInt(result[0])))] = util.tolonglong(result[1]);
		}
		util.uke_list = uke_list_temp;

		tenpai_wei = (double**)malloc(sizeof(double*) * 2);
		for (int i = 0; i<2; i++)
		{
			tenpai_wei[i] = (double*)malloc(sizeof(double)*sum[0]);
			if (tenpai_wei[i] == NULL) { exit(1); }
		}
		matiyomi_wei = (double**)malloc(sizeof(double*) * 34);
		for (int i = 0; i<34; i++)
		{
			matiyomi_wei[i] = (double*)malloc(sizeof(double)*sum[1]);
			if (matiyomi_wei[i] == NULL) { exit(1); }
		}
		tokuten_wei = (double*)malloc(sizeof(double)*sum[2]);
		if (tokuten_wei == NULL) { exit(1); }
		ifs.close();
		ifs.open(execDir + "/source/tenpai_wei.txt");
		if (!ifs) { cout << "tenpai_weiは開けません" << endl; while (1) {} }
		j = 0;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			for (int i = 0; i<(signed)result.size(); i++)
			{
				tenpai_wei[i][j] = util.string2double(result[i]);
			}
			j++;
		}
		ifs.close();
		ifs.open(execDir + "/source/matiyomi_wei.txt");

		if (!ifs) { cout << "matiyomi_weiは開けません" << endl; while (1) {} }
		j = 0;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			for (int i = 0; i<(signed)result.size(); i++)
			{
				matiyomi_wei[i][j] = util.string2double(result[i]);
			}
			j++;
		}
		ifs.close();
		ifs.open(execDir + "/source/tokuten_wei.txt");

		if (!ifs) { cout << "tokuten_weiは開けません" << endl; while (1) {} }
		j = 0;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());

			for (int i = 0; i<(signed)result.size(); i++)
			{
				tokuten_wei[i] = util.string2double(result[i]);
			}
		}
		ifs.close();
		/*ifs.open("sutehai_pro.txt");
		if(!ifs){cout<<"ifsは開けません"<<endl;while(1){}}
		j=0;
		while(getline(ifs,str))
		{
		boost::split(result,str,boost::is_space());
		sutehai_pro[util.toInt(result[0])][util.toInt(result[1])][util.toInt(result[2])]=util.string2double(result[3]);
		//cout<<result[0]<<" "<<result[1]<<" "<<result[2]<<" "<<result[3]<<endl;
		}
		for(int i=0;i<34;i++){for(int j=0;j<34;j++){sutehai_pro[i][4][j]=0.0;}}
		ifs.close();*/
		ifs.open(execDir + "/source/op_seme.txt");
		if (!ifs) { cout << "op_semeは開けません" << endl; while (1) {} }
		j = 0;
		while (getline(ifs, str))
		{
			boost::split(result, str, boost::is_space());
			int index = util.toInt(result[2]);

			for (int j = 3; j <= 7; j++)
			{
				index <<= 2;
				index |= util.toInt(result[j]);
			}

			//cout<<result[0]<<" "<<index<<" "<<util.toInt(result[6])-1<<" "<<val<<endl;
			if (util.toInt(result[8]) - 1 >= 0)
			{
				//cout<<util.toInt(result[0])+5*util.toInt(result[1])<<" "<<index<<" "<<util.toInt(result[8])-1<<" "<<result[11]<<endl;
				n_seme_pro[util.toInt(result[0]) + 5 * util.toInt(result[1])][index][util.toInt(result[8]) - 1] = util.string2double(result[11]);
				n_seme_pro[util.toInt(result[0]) + 5 * util.toInt(result[1])][index][0] = 1.0;
			}
		}
		ifs.close();
	}

	int mieteru_dora(int mieteruhai[], player tatya[], field *game_field)
	{
		int num_dora = 0;
		num_dora += game_field->mieta_aka_hai + tatya[0].aka_fu_ro + tatya[1].aka_fu_ro + tatya[2].aka_fu_ro + tatya[3].aka_fu_ro;
		for (int i = 0; i<5; i++)
		{
			if (game_field->dora[i] == -1) { break; }
			num_dora += mieteruhai[util.get_kind_of_dora(game_field->dora[i])];
		}
		return num_dora;
	}
	int sarasi_dora(player a, field *game_field)
	{
		int i = 0;
		int dora_maisuu = 0;
		int copy[34] = { 0 };
		for (i = 0; i<34; i++)
		{
			copy[i] = 0;
		}
		i = 0;
		while (i<a.fu_ro_suu)
		{
			if (a.fu_ro[i]<34)
			{
				copy[a.fu_ro[i]]++;
				copy[a.fu_ro[i] + 1]++;
				copy[a.fu_ro[i] + 2]++;
			}
			else if (a.fu_ro[i]<68)
			{
				copy[a.fu_ro[i] % 34] += 3;
			}
			else
			{
				copy[a.fu_ro[i] % 34] += 4;
			}
			i++;
			if (i == 4) { break; }
		}

		i = 0;
		while (game_field->dora[i] != -1)
		{
			dora_maisuu += copy[util.get_kind_of_dora(game_field->dora[i])];
			i++;
			if (i == 5)
			{
				break;
			}
		}
		return dora_maisuu + a.aka_fu_ro;
	}
	void make_mieteruhai(int pl, player tatya[], int mieteruhai[], int sutehai[])
	{
        for (int i = 0; i<34; i++)
        {
            mieteruhai[i] = 0;
            //cout << sutehai[i] << "|";
        }
        for (int i = 0; i<34; i++)
		{
			mieteruhai[i] = sutehai[i];
		}
		for (int i = 0; i<34; i++)
		{
            if(tatya[pl].hai[i] >= 0) mieteruhai[i] += tatya[pl].hai[i];
		}

		for (int i = 0; i<4; i++)
		{
			int j = 0;
			while (j<tatya[i].fu_ro_suu)
			{
				if (tatya[i].fu_ro[j]<34)
				{
					mieteruhai[tatya[i].fu_ro[j]]++;
					mieteruhai[tatya[i].fu_ro[j] + 1]++;
					mieteruhai[tatya[i].fu_ro[j] + 2]++;
				}
				else if (tatya[i].fu_ro[j]<68) { mieteruhai[tatya[i].fu_ro[j] % 34] += 3; }
				else { mieteruhai[tatya[i].fu_ro[j] % 34] += 4; }
				j++;
				if (j == 4) { break; }
			}
		}
    }
	int ukeire_bit(int hai[], int fu_ro, int ukeire[])
	{
		int j_m = 0, j_t = 0;
		int tmp = 0, val = 14;
		int shanten[3][9] = { { 0 } };
		int m[3] = { 0 };
		int tarts_over = 0;
		int head[4] = { 0 };
		long long ukeire_val[3] = { 0 };
		for (int iro = 0; iro<3; iro++)
		{
			int j = hai[iro * 9];
			for (int i = 1 + iro * 9; i<9 + iro * 9; ++i) { j <<= 3; j |= hai[i]; }

			int kv = util.shanten_list[j];
			ukeire_val[iro] = util.uke_list[util.IntToString(j)];
			//cout<<iro<<" "<<util.uke_list[j]<<" "<<1045749907231080448<<endl;
			m[iro] = (kv & 0x00000003);
			kv >>= 2;
			for (int i = 0; i<m[iro]; i++)
			{
				shanten[iro][i * 3] = (kv & 0x00000001); kv >>= 1;
				shanten[iro][i * 3 + 1] = (kv & 0x00000007); kv >>= 3;
				shanten[iro][i * 3 + 2] = (kv & 0x00000007); kv >>= 3;

				//cout<<shanten[iro][i*3]<<" "<<shanten[iro][i*3+1]<<" "<<shanten[iro][i*3+2]<<endl;
			}
		}

		for (int i = 27; i<34; ++i)
		{
			if (hai[i] == 0) { continue; }
			else if (hai[i] == 2) { ++j_t; }
			else if (hai[i] >= 3) { ++j_m; }
		}
		for (int k = 0; k<m[0]; ++k)
		{
			for (int kk = 0; kk<m[1]; ++kk)
			{
				for (int kkk = 0; kkk<m[2]; ++kkk)
				{
					int head_ex = (shanten[0][k * 3] + shanten[1][kk * 3] + shanten[2][kkk * 3] + j_t >= 1);
					int mentu = shanten[0][k * 3 + 2] + shanten[1][kk * 3 + 2] + shanten[2][kkk * 3 + 2] + j_m + fu_ro;
					int tarts = shanten[0][k * 3 + 1] + shanten[1][kk * 3 + 1] + shanten[2][kkk * 3 + 1] + j_t + shanten[0][k * 3] + shanten[1][kk * 3] + shanten[2][kkk * 3] - head_ex;
					//cout<<shanten[0][k*3+1]<<shanten[1][kk*3+1]<<shanten[2][kkk*3+1]<<j_t<<shanten[0][k*3]<<shanten[1][kk*3]<<shanten[2][kkk*3]<<head_ex<<endl;
					if (mentu + tarts>4)
					{
						tmp = 8 - 2 * mentu - 4 + mentu - head_ex;
					}//4-mentu-head
					else
					{
						tmp = 8 - 2 * mentu - tarts - head_ex;
					}
					if (val>tmp)
					{
						val = tmp;
						tarts_over = mentu + tarts;
						head[0] = shanten[0][k * 3];
						head[1] = shanten[1][kk * 3];
						head[2] = shanten[2][kkk * 3];
						head[3] = j_t;
					}
				}
			}
		}

		int shift[3] = { 0 };
		if (tarts_over<4) { shift[0] = 54; shift[1] = 54; shift[2] = 54; }
		else if (tarts_over == 4 && head[0] + head[1] + head[2] + head[3] == 0) { shift[0] = 45; shift[1] = 45; shift[2] = 45; }
		else if (tarts_over == 4 && head[0] + head[1] + head[2] + head[3] == 1) { shift[0] = 36; shift[1] = 36; shift[2] = 36; }
		else if (tarts_over == 4 && head[0] + head[1] + head[2] + head[3]>1) { shift[0] = 27 + (head[0] == 0) * 9; shift[1] = 27 + (head[1] == 0) * 9; shift[2] = 27 + (head[2] == 0) * 9; }
		else if (tarts_over>4 && head[0] + head[1] + head[2] + head[3] == 0) { shift[0] = 18; shift[1] = 18; shift[2] = 18; }
		else if (tarts_over>4 && head[0] + head[1] + head[2] + head[3] == 1) { shift[0] = 9; shift[1] = 9; shift[2] = 9; }
		else if (tarts_over>4 && head[0] + head[1] + head[2] + head[3]>1) { shift[0] = (head[0] == 0) * 9; shift[1] = (head[1] == 0) * 9; shift[2] = (head[2] == 0) * 9; }

		//cout<<tarts_over<<" "<<head[0]<<" "<<head[1]<<" "<<head[2]<<" "<<head[3]<<" "<<shift[0]<<" "<<shift[1]<<" "<<shift[2]<<endl;
		if (tarts_over<4 || (head[0] + head[1] + head[2] + head[3] == 0))
		{
			for (int i = 27; i<34; i++)
			{
				if (hai[i] >= 1 && hai[i] <= 2) { ukeire[i] = 1; }
			}
		}
		else if (!((head[0] + head[1] + head[2] + head[3] == 1) && head[3] == 1))
		{
			for (int i = 27; i<34; i++)
			{
				if (hai[i] == 2) { ukeire[i] = 1; }
			}
		}
		for (int i = 0; i<3; i++)
		{
			ukeire_val[i] = ((ukeire_val[i] >> shift[i]) & 0x000001ff);
		}
		for (int i = 0; i<3; i++)
		{
			for (int j = 8; j >= 0; j--)
			{
				ukeire[i * 9 + j] = (ukeire_val[i] & 0x00000001);
				ukeire_val[i] >>= 1;
			}
		}
		return val;
	}
	int frtn(int hai[], int genbutu[], int pai, int fu_ro)
	{
		hai[pai]--;
		for (int i = 0; i<34; i++)
		{
			if (genbutu[i] == 1)
			{
				hai[i]++;
				if (util.shantensuu_all(hai, fu_ro) == -1)
				{
					hai[pai]++;
					hai[i]--;
					return 1;
				}
				hai[i]--;
			}
		}
		hai[pai]++;
		return 0;
	}
	int hai_naki_nuku(int hai[], int hai_copy[], int fu_ro[], int fu_ro_copy[], int pai, int kind)
	{
		int k = 0;
		for (int i = 0; i<34; i++)
		{
			hai_copy[i] = hai[i];
		}
		for (int i = 0; i<4; i++)
		{
			fu_ro_copy[i] = fu_ro[i];
		}
		for (int i = 0; i<4; i++)
		{
			if (fu_ro[i] == -1) { k = i; i = 4; }
		}
		if (kind == 1)
		{
			hai_copy[pai] -= 2;
			fu_ro_copy[k] = pai + 34;
		}
		else if (kind == 2)
		{
			hai_copy[pai] = 0;
			fu_ro_copy[k] = pai + 68;
		}
		else if (kind == 3)
		{
			hai_copy[pai - 1]--;
			hai_copy[pai - 2]--;
			fu_ro_copy[k] = pai - 2;
		}
		else if (kind == 4)
		{
			hai_copy[pai - 1]--;
			hai_copy[pai + 1]--;
			fu_ro_copy[k] = pai - 1;
		}
		else if (kind == 5)
		{
			hai_copy[pai + 1]--;
			hai_copy[pai + 2]--;
			fu_ro_copy[k] = pai;
		}
		return 0;
	}
	int naki_check_push_pattern(int hai[], int player, int aite, int pai, vector<int> *naki_pattern)
	{
		if (hai[pai] >= 2) { naki_pattern->push_back(1); }//1
		if (hai[pai] >= 3) { naki_pattern->push_back(2); }//2

		if ((player + 3) % 4 == aite)
		{
			if (pai<27)
			{
				if (pai % 9 >= 2 && hai[pai - 1]>0 && hai[pai - 2]>0) { naki_pattern->push_back(3); }
				if (pai % 9 >= 1 && pai % 9 <= 7 && hai[pai - 1]>0 && hai[pai + 1]>0) { naki_pattern->push_back(4); }
				if (pai % 9 <= 6 && hai[pai + 1]>0 && hai[pai + 2]>0) { naki_pattern->push_back(5); }
			}
		}

		if (naki_pattern->empty()) { return 0; }
		else { return 1; }
	}
	int cut_aka(int hai[], int pai, int aka_moti)
	{
		int aka[3] = { 0 };
		if (pai >= 27 || pai % 9 != 4) { return 0; }
		else
		{
			if (hai[pai] == 1)
			{
				int i = aka_moti / 4;
				aka[2] = i;
				aka_moti -= i * 4;
				i = aka_moti / 2;
				aka[1] = i;
				aka_moti -= i * 2;
				aka[0] = aka_moti;
				if (pai / 9 == 0 && aka[0] == 1) { return 1; }
				if (pai / 9 == 1 && aka[1] == 1) { return 2; }
				if (pai / 9 == 2 && aka[2] == 1) { return 4; }
			}
		}
		return 0;
	}
	int get_number_of_aka(int aka_moti)
	{
		int i = 0, j = 0;

		i = aka_moti / 4;
		j += i;

		aka_moti -= i * 4;
		i = aka_moti / 2;
		j += i;

		aka_moti -= i * 2;
		j += aka_moti;
		return j;
	}
	int get_number_of_dora2(int hai[], int fu_ro[], field *game_field)
	{
		int i = 0;
		int dora_maisuu = 0;
		int copy[34] = { 0 };
		for (int j = 0; j<34; j++)
		{
			copy[j] = hai[j];
		}
		i = 0;
		while (fu_ro[i] != -1)
		{
			if (fu_ro[i]<34)
			{
				copy[fu_ro[i]]++;
				copy[fu_ro[i] + 1]++;
				copy[fu_ro[i] + 2]++;
			}
			else if (fu_ro[i]<68)
			{
				copy[fu_ro[i] % 34] += 3;
			}
			else
			{
				copy[fu_ro[i] % 34] += 4;
			}
			i++;
			if (i == 4) { break; }
		}

		for (int j = 0; j<5; j++)
		{
			//printf("dora %d %d\n",j,util.get_kind_of_dora(dora[j]));
			if (game_field->dora[j] == -1) { break; }
			dora_maisuu += copy[util.get_kind_of_dora(game_field->dora[j])];
		}
		return dora_maisuu;
	}
	int get_number_of_ura_dora(player tatya[], vector<int> ura)
	{
		if (tatya[0].ri_ti_flag == 0) { return 0; }
		int i = 0;
		int dora_maisuu = 0;
		int copy[34] = { 0 };
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
		}
		i = 0;
		while (tatya[0].fu_ro[i] != -1)
		{
			if (tatya[0].fu_ro[i] >= 102)
			{
				copy[tatya[0].fu_ro[i] % 34] += 4;
			}
			i++;
			if (i == 4) { break; }
		}

		for (int j = 0; j<(signed)ura.size(); j++)
		{
			//printf("dora %d %d\n",j,util.get_kind_of_dora(dora[j]));
			dora_maisuu += copy[util.get_kind_of_dora(ura[j])];
		}
		return dora_maisuu;
	}

	double cal_seme_pro(int pl, player tatya[], int jun, int sp[], int tenpai[])
	{
		int index1 = 0;
		int index2 = 0;
		int index3 = 0;

		if (tatya[pl].ri_ti_flag != 0 || (sp[pl - 1] == 0 && tenpai[pl - 1] == 1)) { return 1.0; }
		else if (tatya[pl].fu_ro_suu != 0) { index1 = tatya[pl].fu_ro_suu + 5 * tenpai[pl - 1]; }
		else if (sp[pl - 1] == 0 && tenpai[pl - 1] == 0) { index1 = 0; }
		else if (sp[pl - 1] == 1) { index1 = 1 + 5 * tenpai[pl - 1]; }

		int fu_ri[6] = { 0 };
		for (int j = 0; j<4; j++)
		{
			if (pl != j)
			{
				if (tatya[j].ri_ti_flag != 0 || (sp[j - 1] == 0 && tenpai[j - 1] == 1)) { fu_ri[5]++; }
				else if (tatya[j].fu_ro_suu == 0 && sp[j - 1] == 1) { fu_ri[1]++; }
				else { fu_ri[tatya[j].fu_ro_suu]++; }
			}
		}
		index2 = fu_ri[0];
		for (int j = 1; j <= 5; j++)
		{
			index2 <<= 2;
			index2 |= fu_ri[j];
		}

		index3 = (signed)tatya[pl].sutehai.size() + jun;
		if (index3>17) { index3 = 17; }
		//cout<<index1<<" "<<index2<<" "<<index3<<" "<<n_seme_pro[index1][index2][index3]<<endl;
		return n_seme_pro[index1][index2][index3];
	}
	int make_feature(player tatya[], field *game_field, int mieteruhai[])
	{
		//return 0;
		int i, j, k, l, ankan = 0, copy[34] = { 0 };
		//int sha=util.shantensuu(tatya[0].hai,tatya[0].fu_ro_suu);
		int ukeire[34] = { 0 };
		int sha = ukeire_bit(tatya[0].hai, tatya[0].fu_ro_suu, ukeire);
		int titoi_sha = util.titoi_shanten(tatya[0].hai, tatya[0].fu_ro_suu);
		int kokusi_sha = util.kokusi_shanten(tatya[0].hai, tatya[0].fu_ro_suu);
		int val = 0;
		int han = 0;

		j = 0;
		for (i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }

		if (tatya[0].fu_ro_suu - ankan == 0) {
			val += op_list[util.IntToString((42 << 22) | 1)]; //feature->push_back("c_r"+util.IntToString(1));
											//value->push_back(op_list[(42<<22)|1]);
		}
		else {
			val += op_list[util.IntToString((42 << 22) | 0)];//feature->push_back("c_r"+util.IntToString(0));
										   //value->push_back(op_list[(42<<22)|0]);
		}
		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		if (sha == 0 || titoi_sha == 0 || kokusi_sha == 0)
		{
			if (sha != 0 || tatya[0].fu_ro_suu - ankan == 0)
			{
				val += op_list[util.IntToString((45 << 22) | 2)];//feature->push_back("y_ari"+util.IntToString(2));
											   //value->push_back(op_list[(45<<22)|2]);
				int key = 0;
				key = 0; key <<= 5; key |= 2; key <<= 5; key |= (game_field->nokori_maisuu / 4);
				val += op_list[util.IntToString((44 << 22) | key)];
				//value->push_back(op_list[(44<<22)|key]);
				//feature->push_back("sh_y_j"+util.IntToString(0)+"_"+util.IntToString(2)+"_"+util.IntToString(game_field->nokori_maisuu/4));
			}
			else
			{
				i = 0; l = 0;

				for (k = 0; k<34; k++)
				{
					if (ukeire[k] * (mieteruhai[k] != 4) == 1)
					{
						int dora[5] = { -1 };
						int han_fu[2] = { 0 };
						tatya[0].hai[k]++;
						if (util.yaku(tatya[0].hai, tatya[0].fu_ro, k, 1, 0, 0, game_field->bafuu, tatya[0].jifuu, 0, dora, 0, 0, 0, 0, han_fu)>0)//�c��
						{
							i = 1;
						}
						else { l = 1; }
						tatya[0].hai[k]--;
					}
				}

				int key = 0; key <<= 5; key |= (1 + i - l); key <<= 5; key |= (game_field->nokori_maisuu / 4);

				val += op_list[util.IntToString((44 << 22) | key)];
				//value->push_back(op_list[(44<<22)|key]);
				//feature->push_back("sh_y_j"+util.IntToString(0)+"_"+util.IntToString(1+i-l)+"_"+util.IntToString(game_field->nokori_maisuu/4));

				val += op_list[util.IntToString((45 << 22) | (1 + i - l))];
				//feature->push_back("y_ari"+util.IntToString(1+i-l));
				//value->push_back(op_list[(45<<22)|(1+i-l)]);
			}
		}
		else
		{
			int key = 1; key <<= 5; key |= 0; key <<= 5; key |= (game_field->nokori_maisuu / 4);
			val += op_list[util.IntToString((44 << 22) | key)];
			//value->push_back(op_list[(44<<22)|key]);
			//feature->push_back("sh_y_j"+util.IntToString(1)+"_"+util.IntToString(0)+"_"+util.IntToString(game_field->nokori_maisuu/4));
		}

		val += op_list[util.IntToString((43 << 22) | (tatya[0].fu_ro_suu << 4) | sha)];
		//value->push_back(op_list[(43<<22)|(tatya[0].fu_ro_suu<<4)|sha]);
		//feature->push_back("sh_f"+util.IntToString(tatya[0].fu_ro_suu)+"_"+util.IntToString(sha));		

		val += op_list[util.IntToString((13 << 22) | (4 << 4) | sha)];

		for (i = 0; i<27; i++)
		{
			k = 0;
			for (j = 0; j<5; j++)
			{
				if (game_field->dora[j] == -1) { break; }
				if (util.get_kind_of_dora(game_field->dora[j]) == i)
				{
					k = 1;
				}
			}
			if (i % 9 <= 4)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= k;
				val += op_list[util.IntToString((15 << 22) | key)];
				//value->push_back(op_list[(15<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+util.IntToString(tatya[0].hai[i])+util.IntToString(k));
			}
			else
			{
				int key = 0;
				key |= (9 - i % 9); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= k;
				val += op_list[util.IntToString((15 << 22) | key)];
				//value->push_back(op_list[(15<<22)|key]);
				//feature->push_back("H"+util.IntToString(9-i%9)+util.IntToString(tatya[0].hai[i])+util.IntToString(k));
			}
		}
		for (i = 27; i<34; i++)
		{
			k = 0;
			for (j = 0; j<5; j++)
			{
				if (game_field->dora[j] == -1) { break; }
				if (util.get_kind_of_dora(game_field->dora[j]) == i)
				{
					k = 1;
				}
			}
			han = 0;
			if (tatya[0].jifuu == i) { han++; }
			if (game_field->bafuu == i) { han++; }
			if (i>30) { han++; }

			int key = 0;
			key |= tatya[0].hai[i]; key <<= 3; key |= han; key <<= 3; key |= k;
			val += op_list[util.IntToString((14 << 22) | key)];
			//value->push_back(op_list[(14<<22)|key]);
			//feature->push_back("H"+util.IntToString(0)+util.IntToString(tatya[0].hai[i])+util.IntToString(han)+util.IntToString(k));
		}

		for (i = 0; i<27; i++)
		{
			if (i % 9 <= 3)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= tatya[0].hai[i + 1];
				val += op_list[util.IntToString((16 << 22) | key)];
				//value->push_back(op_list[(16<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+"_"+util.IntToString(tatya[0].hai[i])+"_"+util.IntToString(tatya[0].hai[i+1]));
			}
			else if (i % 9<8)
			{
				int key = 0;
				key |= (8 - i % 9); key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i];
				val += op_list[util.IntToString((16 << 22) | key)];
				//value->push_back(op_list[(16<<22)|key]);
				//feature->push_back("H"+util.IntToString(8-i%9)+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i]));
			}
		}
		for (i = 0; i<27; i++)
		{
			if (i % 9 <= 3)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i + 2];
				val += op_list[util.IntToString((17 << 22) | key)];
				//value->push_back(op_list[(17<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+"_"+util.IntToString(tatya[0].hai[i])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i+2]));
			}
			else if (i % 9<7)
			{
				int key = 0;
				key |= (7 - i % 9); key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i];
				val += op_list[util.IntToString((17 << 22) | key)];
				//value->push_back(op_list[(17<<22)|key]);
				//feature->push_back("H"+util.IntToString(7-i%9)+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i]));
			}
		}
		for (i = 0; i<27; i++)
		{
			if (i % 9 <= 2)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 3];
				val += op_list[util.IntToString((18 << 22) | key)];
				//value->push_back(op_list[(18<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+"_"+util.IntToString(tatya[0].hai[i])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+3]));
			}
			else if (i % 9<6)
			{
				int key = 0;
				key |= (6 - i % 9); key <<= 3; key |= tatya[0].hai[i + 3]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i];
				val += op_list[util.IntToString((18 << 22) | key)];
				//value->push_back(op_list[(18<<22)|key]);
				//feature->push_back("H"+util.IntToString(6-i%9)+"_"+util.IntToString(tatya[0].hai[i+3])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i]));
			}
		}
		for (i = 0; i<27; i++)
		{
			if (i % 9 <= 2)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 3]; key <<= 3; key |= tatya[0].hai[i + 4];
				val += op_list[util.IntToString((19 << 22) | key)];
				//value->push_back(op_list[(19<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+"_"+util.IntToString(tatya[0].hai[i])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+3])+"_"+util.IntToString(tatya[0].hai[i+4]));
			}
			else if (i % 9<5)
			{
				int key = 0;
				key |= (5 - i % 9); key <<= 3; key |= tatya[0].hai[i + 4]; key <<= 3; key |= tatya[0].hai[i + 3]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i];
				val += op_list[util.IntToString((19 << 22) | key)];
				//value->push_back(op_list[(19<<22)|key]);
				//feature->push_back("H"+util.IntToString(5-i%9)+"_"+util.IntToString(tatya[0].hai[i+4])+"_"+util.IntToString(tatya[0].hai[i+3])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i]));
			}
		}
		for (i = 0; i<27; i++)
		{
			if (i % 9 <= 1)
			{
				int key = 0;
				key |= (i % 9 + 1); key <<= 3; key |= tatya[0].hai[i]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 3]; key <<= 3; key |= tatya[0].hai[i + 4]; key <<= 3; key |= tatya[0].hai[i + 5];
				val += op_list[util.IntToString((20 << 22) | key)];
				//value->push_back(op_list[(20<<22)|key]);
				//feature->push_back("H"+util.IntToString(i%9+1)+"_"+util.IntToString(tatya[0].hai[i])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+3])+"_"+util.IntToString(tatya[0].hai[i+4])+"_"+util.IntToString(tatya[0].hai[i+5]));
			}
			else if (i % 9<4)
			{
				int key = 0;
				key |= (4 - i % 9); key <<= 3; key |= tatya[0].hai[i + 5]; key <<= 3; key |= tatya[0].hai[i + 4]; key <<= 3; key |= tatya[0].hai[i + 3]; key <<= 3; key |= tatya[0].hai[i + 2]; key <<= 3; key |= tatya[0].hai[i + 1]; key <<= 3; key |= tatya[0].hai[i];
				val += op_list[util.IntToString((20 << 22) | key)];
				//value->push_back(op_list[(20<<22)|key]);
				//feature->push_back("H"+util.IntToString(4-i%9)+"_"+util.IntToString(tatya[0].hai[i+5])+"_"+util.IntToString(tatya[0].hai[i+4])+"_"+util.IntToString(tatya[0].hai[i+3])+"_"+util.IntToString(tatya[0].hai[i+2])+"_"+util.IntToString(tatya[0].hai[i+1])+"_"+util.IntToString(tatya[0].hai[i]));
			}
		}

		{
			for (i = 0; i<27; i++)
			{
				if (tatya[0].hai[i] >= 2) { copy[i] = 2; }
				else if (tatya[0].hai[i] == 1) { copy[i] = 1; }
				else { copy[i] = 0; }
			}

			int key = 0;
			for (int i = 0; i<27; i++)
			{
				if (i % 9 == 0)
				{
					key = 0;
					key = copy[i];
				}
				else
				{
					key <<= 2;
					key |= copy[i];
				}
				if (i % 9 == 8)
				{
					val += op_list[util.IntToString((1 << 22) | key)];
					//value->push_back(op_list[(1<<22)|key]);
				}
			}

			//feature->push_back("it"+util.IntToString(copy[0])+util.IntToString(copy[1])+util.IntToString(copy[2])+util.IntToString(copy[3])+util.IntToString(copy[4])+util.IntToString(copy[5])+util.IntToString(copy[6])+util.IntToString(copy[7])+util.IntToString(copy[8]));
			//feature->push_back("it"+util.IntToString(copy[9])+util.IntToString(copy[10])+util.IntToString(copy[11])+util.IntToString(copy[12])+util.IntToString(copy[13])+util.IntToString(copy[14])+util.IntToString(copy[15])+util.IntToString(copy[16])+util.IntToString(copy[17]));
			//feature->push_back("it"+util.IntToString(copy[18])+util.IntToString(copy[19])+util.IntToString(copy[20])+util.IntToString(copy[21])+util.IntToString(copy[22])+util.IntToString(copy[23])+util.IntToString(copy[24])+util.IntToString(copy[25])+util.IntToString(copy[26]));
		}

		j = 0; k = 0;
		for (i = 0; i<9; i++)
		{
			k += tatya[0].hai[i];
		}
		for (i = 9; i<18; i++)
		{
			j += tatya[0].hai[i];
			if (j>k)
			{
				k = j;
			}
		}
		j = 0;
		for (i = 18; i<27; i++)
		{
			j += tatya[0].hai[i];
			if (j>k)
			{
				k = j;
			}
		}
		j = 0;
		for (i = 27; i<34; i++)
		{
			j += tatya[0].hai[i];
		}
		val += op_list[util.IntToString((2 << 22) + k)];
		//value->push_back(op_list[(2<<22)+k]);
		//feature->push_back("max_iro_t"+util.IntToString(k));
		val += op_list[util.IntToString((3 << 22) + k + j)];
		//value->push_back(op_list[(3<<22)+k+j]);
		//feature->push_back("max_iro_h"+util.IntToString(k+j));

		val += op_list[util.IntToString((4 << 22) + titoi_sha)];
		//value->push_back(op_list[(4<<22)+titoi_shanten(tatya[0].hai)]);
		//feature->push_back("ti-toi"+util.IntToString(titoi_shanten(tatya[0].hai)));
		val += op_list[util.IntToString((5 << 22) + kokusi_sha)];
		//value->push_back(op_list[(5<<22)+kokusi_shanten(tatya[0].hai)]);
		//feature->push_back("kokusi"+util.IntToString(kokusi_shanten(tatya[0].hai)));

		val += op_list[util.IntToString((12 << 22) + tatya[0].hai[0] + tatya[0].hai[8] + tatya[0].hai[9] + tatya[0].hai[17] + tatya[0].hai[18] + tatya[0].hai[26] + tatya[0].hai[27] + tatya[0].hai[28] + tatya[0].hai[29] + tatya[0].hai[30] + tatya[0].hai[31] + tatya[0].hai[32] + tatya[0].hai[33])];
		//value->push_back(op_list[(12<<22)+tatya[0].hai[0]+tatya[0].hai[8]+tatya[0].hai[9]+tatya[0].hai[17]+tatya[0].hai[18]+tatya[0].hai[26]+tatya[0].hai[27]+tatya[0].hai[28]+tatya[0].hai[29]+tatya[0].hai[30]+tatya[0].hai[31]+tatya[0].hai[32]+tatya[0].hai[33]]);

		///feature->push_back("yao"+util.IntToString(tatya[0].hai[0]+tatya[0].hai[8]+tatya[0].hai[9]+tatya[0].hai[17]+tatya[0].hai[18]+tatya[0].hai[26]+tatya[0].hai[27]+tatya[0].hai[28]+tatya[0].hai[29]+tatya[0].hai[30]+tatya[0].hai[31]+tatya[0].hai[32]+tatya[0].hai[33]));
		j = sha;
		copy[0] = 0; copy[1] = 0; copy[2] = 0; copy[3] = 0;
		for (int i = 0; i<34; i++)
		{
			copy[i / 9] += (4 - mieteruhai[i])*ukeire[i];
		}
		if (j == 0)
		{
			//j=shanten_maisuu(tatya[0].hai,mieteruhai,-1,j,tatya[0].fu_ro_suu);
			j = copy[0] + copy[1] + copy[2] + copy[3];
			if (j>11) { j = 11; }
			val += op_list[util.IntToString((7 << 22) + j)];
			//value->push_back(op_list[(7<<22)+j]);

			//feature->push_back("uke0_"+util.IntToString(j));
		}
		else if (j == 1)
		{

			//j=shanten_maisuu(tatya[0].hai,mieteruhai,-1,j,tatya[0].fu_ro_suu);
			j = copy[0] + copy[1] + copy[2] + copy[3];
			if (j>16) { j = 16; }
			val += op_list[util.IntToString((8 << 22) + j)];
			//value->push_back(op_list[(8<<22)+j]);

			//feature->push_back("uke1_"+util.IntToString(j));
		}
		/*copy[0]=0;copy[1]=0;copy[2]=0;copy[3]=0;
		for(i=0;i<34;i++)
		{
		if(mieteruhai[i]<4 && ukihai_x(tatya[0].hai,i)==1)
		{
		tatya[0].hai[i]++;
		if(sha>util.shantensuu(tatya[0].hai,tatya[0].fu_ro_suu))
		{
		copy[i/9]+=4-mieteruhai[i];
		}
		tatya[0].hai[i]--;
		}
		}*/
		for (i = 0; i<4; i++)
		{
			if (copy[i]>11)
			{
				copy[i] = 11;
			}
		}
		for (i = 0; i<4; i++)
		{
			for (j = 1; j<4 - i; j++)
			{
				if (copy[j - 1]>copy[j])
				{
					k = copy[j];
					copy[j] = copy[j - 1];
					copy[j - 1] = k;
				}
			}
		}

		val += op_list[util.IntToString((6 << 22) + copy[0])];//value->push_back(op_list[(6<<22)+copy[0]]);//feature->push_back("uke_"+util.IntToString(copy[0]));
		val += op_list[util.IntToString((6 << 22) + copy[1])];//value->push_back(op_list[(6<<22)+copy[1]]);//feature->push_back("uke_"+util.IntToString(copy[1]));
		val += op_list[util.IntToString((6 << 22) + copy[2])];//value->push_back(op_list[(6<<22)+copy[2]]);//feature->push_back("uke_"+util.IntToString(copy[2]));
		val += op_list[util.IntToString((6 << 22) + copy[3])];//value->push_back(op_list[(6<<22)+copy[3]]);//feature->push_back("uke_"+util.IntToString(copy[3]));

		val += op_list[util.IntToString((9 << 22) + (copy[0] << 4) + copy[1])];//value->push_back(op_list[(9<<22)+(copy[0]<<4)+copy[1]]);//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[1]));
		val += op_list[util.IntToString((9 << 22) + (copy[0] << 4) + copy[2])];//value->push_back(op_list[(9<<22)+(copy[0]<<4)+copy[2]]);//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[2]));
		val += op_list[util.IntToString((9 << 22) + (copy[0] << 4) + copy[3])];//value->push_back(op_list[(9<<22)+(copy[0]<<4)+copy[3]]);//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[3]));
		val += op_list[util.IntToString((9 << 22) + (copy[1] << 4) + copy[2])];//value->push_back(op_list[(9<<22)+(copy[1]<<4)+copy[2]]);//feature->push_back("uke_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[2]));
		val += op_list[util.IntToString((9 << 22) + (copy[1] << 4) + copy[3])];//value->push_back(op_list[(9<<22)+(copy[1]<<4)+copy[3]]);//feature->push_back("uke_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[3]));
		val += op_list[util.IntToString((9 << 22) + (copy[2] << 4) + copy[3])];//value->push_back(op_list[(9<<22)+(copy[2]<<4)+copy[3]]);//feature->push_back("uke_"+util.IntToString(copy[2])+"_"+util.IntToString(copy[3]));

		val += op_list[util.IntToString((10 << 22) + (copy[0] << 8) + (copy[1] << 4) + (copy[2]))];
		//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[2]));
		//value->push_back(op_list[(10<<22)+(copy[0]<<8)+(copy[1]<<4)+(copy[2])]);
		val += op_list[util.IntToString((10 << 22) + (copy[0] << 8) + (copy[1] << 4) + (copy[3]))];
		//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[3]));
		//value->push_back(op_list[(10<<22)+(copy[0]<<8)+(copy[1]<<4)+(copy[3])]);
		val += op_list[util.IntToString((10 << 22) + (copy[0] << 8) + (copy[2] << 4) + (copy[3]))];
		//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[2])+"_"+util.IntToString(copy[3]));
		//value->push_back(op_list[(10<<22)+(copy[0]<<8)+(copy[2]<<4)+(copy[3])]);
		val += op_list[util.IntToString((10 << 22) + (copy[1] << 8) + (copy[2] << 4) + (copy[3]))];
		//feature->push_back("uke_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[2])+"_"+util.IntToString(copy[3]));
		//value->push_back(op_list[(10<<22)+(copy[1]<<8)+(copy[2]<<4)+(copy[3])]);

		val += op_list[util.IntToString((11 << 22) + (copy[0] << 12) + (copy[1] << 8) + (copy[2] << 4) + copy[3])];
		//feature->push_back("uke_"+util.IntToString(copy[0])+"_"+util.IntToString(copy[1])+"_"+util.IntToString(copy[2])+"_"+util.IntToString(copy[3]));
		//value->push_back(op_list[(11<<22)+(copy[0]<<12)+(copy[1]<<8)+(copy[2]<<4)+copy[3]]);

		k = -1; han = -1;
		//i=ryanmen_mentusuu(tatya[0].hai,mieteruhai,&k,&han);
		util.ryanmen_block(tatya[0].hai, &k, &han);
		val += op_list[util.IntToString((21 << 22) + k + tatya[0].fu_ro_suu)];
		val += op_list[util.IntToString((22 << 22) + han + tatya[0].fu_ro_suu)];
		val += op_list[util.IntToString((23 << 22) + ((k + tatya[0].fu_ro_suu) << 4) + han + tatya[0].fu_ro_suu)];

		//j=block(tatya[0].hai);
		//val+=op_list[(21<<22)+i+tatya[0].fu_ro_suu];
		//val+=op_list[(22<<22)+j+tatya[0].fu_ro_suu];	
		//val+=op_list[(23<<22)+((i+tatya[0].fu_ro_suu)<<4)+j+tatya[0].fu_ro_suu];

		for (i = 0; i<4; i++)
		{
			if (tatya[0].fu_ro[i] != -1) {
				val += op_list[util.IntToString((24 << 22) + tatya[0].fu_ro[i])];
				//value->push_back(op_list[(24<<22)+tatya[0].fu_ro[i]]);

				//feature->push_back("f"+util.IntToString(tatya[0].fu_ro[i]));
			}
		}
		for (i = 0; i<3; i++)
		{
			for (j = i + 1; j<4; j++)
			{
				if (tatya[0].fu_ro[j] != -1)
				{
					if (tatya[0].fu_ro[i]<tatya[0].fu_ro[j]) {
						val += op_list[util.IntToString((25 << 22) + (tatya[0].fu_ro[i] << 8) + tatya[0].fu_ro[j])];
						//feature->push_back("f"+util.IntToString(tatya[0].fu_ro[i])+"_"+util.IntToString(tatya[0].fu_ro[j]));
						//value->push_back(op_list[(25<<22)+(tatya[0].fu_ro[i]<<8)+tatya[0].fu_ro[j]]);

					}
					else {
						val += op_list[util.IntToString((25 << 22) + (tatya[0].fu_ro[j] << 8) + tatya[0].fu_ro[i])];
						//value->push_back(op_list[(25<<22)+(tatya[0].fu_ro[j]<<8)+tatya[0].fu_ro[i]]);

						//feature->push_back("f"+util.IntToString(tatya[0].fu_ro[j])+"_"+util.IntToString(tatya[0].fu_ro[i]));
					}
				}
			}
		}
		j = 0; k = 0; l = 0;
		for (i = 0; i<34; i++)
		{
			if (tatya[0].hai[i] >= 3) { j++; }
			else if (tatya[0].hai[i] >= 2) { l++; }
		}
		val += op_list[util.IntToString((26 << 22) + (j << 4) + l)];//feature->push_back("ko_m"+util.IntToString(j)+"_"+util.IntToString(l));
												  //value->push_back(op_list[(26<<22)+(j<<4)+l]);

		for (i = 0; i<4; i++)
		{
			if (tatya[0].fu_ro[i] >= 34 && k != -1) { k++; }
			else if (tatya[0].fu_ro[i] >= 0) { k = -1; }//�`�[����������
		}
		if (k != -1) {
			val += op_list[util.IntToString((28 << 22) + ((j + k) << 4) + l)];//value->push_back(op_list[(28<<22)+((j+k)<<4)+l]);
															//feature->push_back("ko_f"+util.IntToString(j+k)+"_"+util.IntToString(l));
		}
		else {
			val += op_list[util.IntToString((27 << 22) + 0)];//feature->push_back("ko_f_m");
										   //value->push_back(op_list[(27<<22)+0]);
		}

		j = 0;
		for (i = 0; i<27; i++) { if (i % 9 != 0 && i % 9 != 8) { j += tatya[0].hai[i]; } }
		i = 0;
		while (i<tatya[0].fu_ro_suu)
		{
			if (j != -1 && ((tatya[0].fu_ro[i]<34 && tatya[0].fu_ro[i] % 9 != 0 && tatya[0].fu_ro[i] % 9 != 6) ||
				(tatya[0].fu_ro[i] >= 34 && tatya[0].fu_ro[i] % 34<27 && (tatya[0].fu_ro[i] % 34) % 9 != 0 && (tatya[0].fu_ro[i] % 34) % 9 != 8)))
			{
				j += 3;
			}
			else
			{
				j = -1;
			}
			i++;
		}
		{
			int key = j;
			if (key == -1) { key = 0xf; }
			val += op_list[util.IntToString((29 << 22) + key)];
			//value->push_back(op_list[(29<<22)+key]);

			//feature->push_back("tan_f"+util.IntToString(j));
		}
		for (i = 0; i<34; i++) { copy[i] = tatya[0].hai[i]; }
		copy[0] = 0; copy[8] = 0; copy[9] = 0; copy[17] = 0; copy[18] = 0; copy[26] = 0;
		copy[27] = 0; copy[28] = 0; copy[29] = 0; copy[30] = 0; copy[31] = 0; copy[32] = 0; copy[33] = 0;
		if (j == -1) {
			val += op_list[util.IntToString((30 << 22) + 0)];//feature->push_back("tan_sh_m");value->push_back(op_list[(30<<22)+0]);

		}
		else {
			val += op_list[util.IntToString((31 << 22) + util.shantensuu(copy, tatya[0].fu_ro_suu))];//value->push_back(op_list[(31<<22)+shantensuu2(copy,tatya[0].fu_ro_suu)]);
																			  //feature->push_back("tan_sh"+util.IntToString(shantensuu2(copy,tatya[0].fu_ro_suu)));
		}

		i = get_number_of_dora2(copy, tatya[0].fu_ro, game_field) + get_number_of_aka(tatya[0].aka_moti) + tatya[0].aka_fu_ro;
		if (i>3) { i = 3; }
		val += op_list[util.IntToString((41 << 22) + i)];
		//value->push_back(op_list[(41<<22)+i]);
		//feature->push_back("tan_do"+util.IntToString(i));


		k = ukeire[0] * (mieteruhai[0]<4) + ukeire[8] * (mieteruhai[8]<4) + ukeire[9] * (mieteruhai[9]<4) + ukeire[17] * (mieteruhai[17]<4) + ukeire[18] * (mieteruhai[18]<4) + ukeire[26] * (mieteruhai[26]<4) + ukeire[27] * (mieteruhai[27]<4) + ukeire[28] * (mieteruhai[28]<4) + ukeire[29] * (mieteruhai[29]<4) + ukeire[30] * (mieteruhai[30]<4) + ukeire[31] * (mieteruhai[31]<4) + ukeire[32] * (mieteruhai[32]<4) + ukeire[33] * (mieteruhai[33]<4);

		/*for(i=0;i<34;i++)
		{
		if( (i%9==0 || i%9==8 || i>=27) && mieteruhai[i]<4 && ukihai_x(tatya[0].hai,i)==1)
		{
		tatya[0].hai[i]++;
		if(sha>util.shantensuu(tatya[0].hai,tatya[0].fu_ro_suu)){k++;}
		tatya[0].hai[i]--;
		}
		}*/
		if (k >= 2) { k = 2; }
		if (j == -1 || (tatya[0].fu_ro_suu - ankan == 0 && j != -1))//�^�����I�s���Ȃ��� ���Ă��Ȃ�
		{
			val += op_list[util.IntToString((47 << 22) + (0 << 4) + k)];
			//value->push_back(op_list[util.IntToString((47<<22)+(0<<4)+k]);

			//feature->push_back("tan_19_"+util.IntToString(0)+"_"+util.IntToString(k));
		}
		else//�^�����I�s�����
		{
			val += op_list[util.IntToString((47 << 22) + (1 << 4) + k)];
			//value->push_back(op_list[util.IntToString((47<<22)+(1<<4)+k]);

			//feature->push_back("tan_19_"+util.IntToString(1)+"_"+util.IntToString(k));
		}

		if (tatya[0].fu_ro_suu - ankan != 0 && j != -1)
		{
			l = sha;
			for (i = 0; i<34; i++) { copy[i] = tatya[0].hai[i]; }
			copy[0] = 0; copy[8] = 0; copy[9] = 0; copy[17] = 0; copy[18] = 0; copy[26] = 0;
			copy[27] = 0; copy[28] = 0; copy[29] = 0; copy[30] = 0; copy[31] = 0; copy[32] = 0; copy[33] = 0;
			k = util.shantensuu(copy, tatya[0].fu_ro_suu);
			if (k - l>2) {
				val += op_list[util.IntToString((48 << 22) + 2)];//value->push_back(op_list[util.IntToString((48<<22)+2]);
											   //feature->push_back("tan_-19_sh"+util.IntToString(2));
			}
			else {
				val += op_list[util.IntToString((48 << 22) + k - l)];//value->push_back(op_list[util.IntToString((48<<22)+k-l]);feature->push_back("tan_-19_sh"+util.IntToString(k-l));
			}
		}

		for (i = 0; i<27; i++)
		{
			if (tatya[0].hai[i] >= 1) { copy[i] = 1; }
			else { copy[i] = 0; }
		}
		for (i = 1; i <= 7; i++)
		{
			int key = 0;
			if (i == 1) { key <<= 2; key |= copy[i - 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i + 1]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[18 + i - 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i + 1]; }
			else if (i == 7) { key <<= 2; key |= copy[i + 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i - 1]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[18 + i + 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i - 1]; }
			else { key = 1; key <<= 2; key |= copy[i - 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i + 1]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[18 + i - 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i + 1]; }
			val += op_list[util.IntToString((33 << 22) + key)];//value->push_back(op_list[util.IntToString((33<<22)+key]);

			if (i == 1) {//feature->push_back("san"+util.IntToString(0)+util.IntToString(copy[i-1])+util.IntToString(copy[i])+util.IntToString(copy[i+1])+util.IntToString(copy[9+i-1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i+1])+util.IntToString(copy[18+i-1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i+1]));
			}
			else if (i == 7) {//feature->push_back("san"+util.IntToString(0)+util.IntToString(copy[i+1])+util.IntToString(copy[i])+util.IntToString(copy[i-1])+util.IntToString(copy[9+i+1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i-1])+util.IntToString(copy[18+i+1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i-1]));
			}
			else {//feature->push_back("san"+util.IntToString(1)+util.IntToString(copy[i-1])+util.IntToString(copy[i])+util.IntToString(copy[i+1])+util.IntToString(copy[9+i-1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i+1])+util.IntToString(copy[18+i-1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i+1]));
			}
		}

		for (i = 1; i <= 7; i++)
		{
			for (k = 0; k<27; k++)
			{
				if (tatya[0].hai[k] >= 1) { copy[k] = 1; }
				else { copy[k] = 0; }
			}
			j = 0;
			while (j<tatya[0].fu_ro_suu)
			{
				if (tatya[0].fu_ro[j]<34 && tatya[0].fu_ro[j] % 9 + 1 == i) { copy[tatya[0].fu_ro[j]] = 1; copy[tatya[0].fu_ro[j] + 1] = 1; copy[tatya[0].fu_ro[j] + 2] = 1; }
				j++;
			}

			int key = 0;
			if (i == 1) { key |= copy[i - 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i + 1]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[18 + i - 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i + 1]; }
			else if (i == 7) { key |= copy[i + 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i - 1]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[18 + i + 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i - 1]; }
			else { key = 1; key <<= 2; key |= copy[i - 1]; key <<= 2; key |= copy[i]; key <<= 2; key |= copy[i + 1]; key <<= 2; key |= copy[9 + i - 1]; key <<= 2; key |= copy[9 + i]; key <<= 2; key |= copy[9 + i + 1]; key <<= 2; key |= copy[18 + i - 1]; key <<= 2; key |= copy[18 + i]; key <<= 2; key |= copy[18 + i + 1]; }
			val += op_list[util.IntToString((32 << 22) + key)];//value->push_back(op_list[util.IntToString((32<<22)+key]);

			if (i == 1) {//feature->push_back("san_f"+util.IntToString(0)+util.IntToString(copy[i-1])+util.IntToString(copy[i])+util.IntToString(copy[i+1])+util.IntToString(copy[9+i-1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i+1])+util.IntToString(copy[18+i-1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i+1]));
			}
			else if (i == 7) {//feature->push_back("san_f"+util.IntToString(0)+util.IntToString(copy[i+1])+util.IntToString(copy[i])+util.IntToString(copy[i-1])+util.IntToString(copy[9+i+1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i-1])+util.IntToString(copy[18+i+1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i-1]));
			}
			else {//feature->push_back("san_f"+util.IntToString(1)+util.IntToString(copy[i-1])+util.IntToString(copy[i])+util.IntToString(copy[i+1])+util.IntToString(copy[9+i-1])+util.IntToString(copy[9+i])+util.IntToString(copy[9+i+1])+util.IntToString(copy[18+i-1])+util.IntToString(copy[18+i])+util.IntToString(copy[18+i+1]));
			}
		}

		for (i = 0; i<27; i++)
		{
			if (tatya[0].hai[i] >= 1) { copy[i] = 1; }
			else { copy[i] = 0; }
		}
		j = 0;
		while (j<tatya[0].fu_ro_suu)
		{
			if (tatya[0].fu_ro[j]<34 && (tatya[0].fu_ro[j] % 9 == 0 || tatya[0].fu_ro[j] % 9 == 3 || tatya[0].fu_ro[j] % 9 == 6)) { copy[tatya[0].fu_ro[j]] = 1; copy[tatya[0].fu_ro[j] + 1] = 1; copy[tatya[0].fu_ro[j] + 2] = 1; }
			j++;
		}

		{
			int key = 0;
			for (int i = 0; i<27; i++)
			{
				if (i % 9 == 0)
				{
					key = 0;
					key = copy[i];
				}
				else
				{
					key <<= 2;
					key |= copy[i];
				}
				if (i % 9 == 8)
				{
					val += op_list[util.IntToString((34 << 22) + key)];
					//value->push_back(op_list[util.IntToString((34<<22)+key]);
				}
			}
			//feature->push_back("it_f"+util.IntToString(copy[0])+util.IntToString(copy[1])+util.IntToString(copy[2])+util.IntToString(copy[3])+util.IntToString(copy[4])+util.IntToString(copy[5])+util.IntToString(copy[6])+util.IntToString(copy[7])+util.IntToString(copy[8]));
			//feature->push_back("it_f"+util.IntToString(copy[9])+util.IntToString(copy[10])+util.IntToString(copy[11])+util.IntToString(copy[12])+util.IntToString(copy[13])+util.IntToString(copy[14])+util.IntToString(copy[15])+util.IntToString(copy[16])+util.IntToString(copy[17]));
			//feature->push_back("it_f"+util.IntToString(copy[18])+util.IntToString(copy[19])+util.IntToString(copy[20])+util.IntToString(copy[21])+util.IntToString(copy[22])+util.IntToString(copy[23])+util.IntToString(copy[24])+util.IntToString(copy[25])+util.IntToString(copy[26]));
		}

		val += op_list[util.IntToString((35 << 22) + tatya[0].fu_ro_suu)];
		//value->push_back(op_list[util.IntToString((35<<22)+tatya[0].fu_ro_suu]);
		//feature->push_back("fn"+util.IntToString(tatya[0].fu_ro_suu));

		j = 0; i = 0; k = -1;
		while (j<tatya[0].fu_ro_suu)
		{
			if (i != -1 && tatya[0].fu_ro[j] % 34 >= 27) { i += 3; }
			else if (k == -1) { k = (tatya[0].fu_ro[j] % 34) / 9; i += 3; }
			else if (k != -1 && k != (tatya[0].fu_ro[j] % 34) / 9) { i = -1; }
			else if (k == (tatya[0].fu_ro[j] % 34) / 9) { i += 3; }
			j++;
		}
		if (i != -1)
		{
			for (j = 0; j<34; j++) { copy[j] = tatya[0].hai[j]; }
			int tmp = 0;
			if (k == -1)
			{
				int iro = 0;
				k = 0;
				for (j = 0; j<9; j++) { tmp += tatya[0].hai[j]; }
				for (j = 9; j<18; j++) { k += tatya[0].hai[j]; }
				if (tmp<k) { tmp = k; iro = 1; }
				k = 0;
				for (j = 18; j<27; j++) { k += tatya[0].hai[j]; }
				if (tmp<k) { tmp = k; iro = 2; }
				i += tmp;
				for (j = 0; j<27; j++) { if (j / 9 != iro) { copy[j] = 0; } }
			}
			else
			{
				for (j = 9 * k; j<9 * k + 9; j++) { i += tatya[0].hai[j]; }
				for (j = 0; j<27; j++) { if (j / 9 != k) { copy[j] = 0; } }
			}
			for (j = 27; j<34; j++) { i += tatya[0].hai[j]; }
		}

		{
			int key = i;
			if (key == -1) { key = 0xf; }
			val += op_list[util.IntToString((37 << 22) + key)];
			//value->push_back(op_list[util.IntToString((37<<22)+key]);
			//feature->push_back("max_iro_h_f"+util.IntToString(i));

			if (key == 0xf) {
				val += op_list[util.IntToString((38 << 22) + key)];//value->push_back(op_list[util.IntToString((38<<22)+key]);
			}
			else {
				val += op_list[util.IntToString((38 << 22) + util.shantensuu(copy, tatya[0].fu_ro_suu))];//value->push_back(op_list[util.IntToString((38<<22)+shantensuu2(copy,tatya[0].fu_ro_suu)]);
			}

			//if(i==-1){feature->push_back("some_sh"+util.IntToString(-1));}
			//else{feature->push_back("some_sh"+util.IntToString(shantensuu2(copy,tatya[0].fu_ro_suu)));}
		}

		j = 0; i = 0; k = -1;
		while (j<tatya[0].fu_ro_suu)
		{
			if (tatya[0].fu_ro[j] % 34 >= 27) { i = -1; }
			else if (k == -1) { k = (tatya[0].fu_ro[j] % 34) / 9; i += 3; }
			else if (k != -1 && k != (tatya[0].fu_ro[j] % 34) / 9) { i = -1; }
			else if (k == (tatya[0].fu_ro[j] % 34) / 9) { i += 3; }
			j++;
		}
		if (i != -1)
		{
			int tmp = 0;
			if (k == -1)
			{
				k = 0;
				for (j = 0; j<9; j++) { tmp += tatya[0].hai[j]; }
				for (j = 9; j<18; j++) { k += tatya[0].hai[j]; }
				if (tmp<k) { tmp = k; }
				k = 0;
				for (j = 18; j<27; j++) { k += tatya[0].hai[j]; }
				if (tmp<k) { tmp = k; }
				i += tmp;
			}
			else
			{
				for (j = 9 * k; j<9 * k + 9; j++) { i += tatya[0].hai[j]; }
			}
		}

		{
			int key = i;
			if (key == -1) { key = 0xf; }
			val += op_list[util.IntToString((36 << 22) + key)];
			//value->push_back(op_list[util.IntToString((36<<22)+key]);
			//feature->push_back("max_iro_t_f"+util.IntToString(i));
		}
		j = 0; i = 0;
		while (j<tatya[0].fu_ro_suu)
		{
			if (tatya[0].fu_ro[j] % 34 == tatya[0].jifuu) { i++; }
			if (tatya[0].fu_ro[j] % 34 == game_field->bafuu) { i++; }
			if (tatya[0].fu_ro[j] % 34>30) { i++; }
			j++;
		}
		val += op_list[util.IntToString((39 << 22) + i)];
		//value->push_back(op_list[util.IntToString((39<<22)+i]);
		//feature->push_back("yaku"+util.IntToString(i));

		i = get_number_of_dora2(tatya[0].hai, tatya[0].fu_ro, game_field) + get_number_of_aka(tatya[0].aka_moti) + tatya[0].aka_fu_ro;
		if (i>3) { i = 3; }
		val += op_list[util.IntToString((40 << 22) + i)];
		//value->push_back(op_list[util.IntToString((40<<22)+i]);
		//feature->push_back("do"+util.IntToString(i));
		j = 0;
		for (i = 0; i<31; i++)
		{
			if (tatya[0].hai[i] >= 2 && i != tatya[0].jifuu && i != game_field->bafuu)
			{
				j++;
			}
		}
		val += op_list[util.IntToString((46 << 22) + j)];
		//value->push_back(op_list[util.IntToString((46<<22)+j]);
		//feature->push_back("ata-ya"+util.IntToString(j));		

		for (i = 27; i<34; i++)
		{
			copy[i] = tatya[0].hai[i];
			if (copy[i]>3) { copy[i] = 3; }
		}
		j = 0;
		while (j<tatya[0].fu_ro_suu)
		{
			if (tatya[0].fu_ro[j] % 34 >= 27) { copy[tatya[0].fu_ro[j] % 34] = 3; }
			j++;
		}

		{
			int key = 0;
			key |= copy[27]; key <<= 4; key |= copy[28]; key <<= 4; key |= copy[29]; key <<= 4; key |= copy[30];
			val += op_list[util.IntToString((49 << 22) + key)];

			//value->push_back(op_list[util.IntToString((49<<22)+key]);

			//feature->push_back("kaze"+util.IntToString(copy[27])+util.IntToString(copy[28])+util.IntToString(copy[29])+util.IntToString(copy[30]));
			key = 0;
			key |= copy[31]; key <<= 4; key |= copy[32]; key <<= 4; key |= copy[33];
			val += op_list[util.IntToString((50 << 22) + key)];
			//value->push_back(op_list[util.IntToString((50<<22)+key]);
			//feature->push_back("sangen"+util.IntToString(copy[31])+util.IntToString(copy[32])+util.IntToString(copy[33]));
		}
		j = 0; l = sha;
		for (int it = 0; it<34; it++)
		{
			if (tatya[0].genbutu[it] == 1 && ukeire[it] == 1)
			{
				j = 1;
				break;
			}
		}

		if (l >= 3) { l = 3; }

		{
			int key = 0;
			key |= tatya[0].fu_ro_suu; key <<= 4; key |= l; key <<= 4; key |= j;
			val += op_list[util.IntToString((51 << 22) + key)];
			//value->push_back(op_list[util.IntToString((51<<22)+key]);
			//feature->push_back("furi"+util.IntToString(tatya[0].fu_ro_suu)+util.IntToString(l)+util.IntToString(j));
		}

		return val;
	}

	int move_tsumo(player tatya[], field *game_field, vector<int> *mv_rank)
	{
		int sutehai[34] = { 0 };
		for (int i = 0; i<37; i++)
		{
            if(i%10==0)continue;
			sutehai[util.trance37to34(i)] += game_field->sutehai_sarasi[i];
		}
		int mieteruhai[34] = { 0 };
		make_mieteruhai(0, tatya, mieteruhai, sutehai);
		//util.hai_check(tatya[0].hai);
		double tenpai_pro[3] = { 0.0 };
		boost::unordered_map<string, double> matiyomi_pro;
		boost::unordered_map<string, double> tokuten_pro;
		boost::unordered_map<string, double> tokuten_pro_tsumo;

		//cal_tenpai_pro(tenpai_pro, tenpai_wei, 0, tatya, game_field, sutehai);
		matiyomi_pro = cal_matiyomi_pro( matiyomi_wei, 0, tatya, game_field, mieteruhai);

		cal_tokuten_pro(tokuten_pro_tsumo, tokuten_wei, 0, tatya, game_field, mieteruhai);

		double tktn[6][34] = { 0.0 };

		int cp_flag[3] = { 0 };
		for (int i = 1; i<4; i++) { cp_flag[i - 1] = tatya[i].ri_ti_flag; }
		for (int i = 1; i<4; i++) { if (tatya[i].fu_ro_suu == 0) { tatya[i].ri_ti_flag = 1; } }
		cal_tokuten_pro(tokuten_pro, tokuten_wei, 0, tatya, game_field, mieteruhai);
		for (int i = 1; i<4; i++) { tatya[i].ri_ti_flag = cp_flag[i - 1]; }
		for (int i = 0; i<3; i++) { for (int j = 0; j<34; j++) { tktn[i][j] = tokuten_pro[util.IntToString(i + 1) + "_" + util.IntToString(j)]; } }
		cal_tokuten_pro(tokuten_pro, tokuten_wei, 0, tatya, game_field, mieteruhai);
		for (int i = 0; i<3; i++) { for (int j = 0; j<34; j++) { tktn[i + 3][j] = tokuten_pro[util.IntToString(i + 1) + "_" + util.IntToString(j)]; } }

		double pass_pro[3][34] = { { 0.0 } };
		for (int i = 0; i<34; i++) { for (int j = 0; j<3; j++) { pass_pro[j][i] = (1.0 - matiyomi_pro[util.IntToString(j + 1) + "_" + util.IntToString(i)]); } }

		vector<int> gouhoushu1;
		vector<double> exp1;
		vector<double> tumorare1;

		player tatya_pl[4];
		for (int i = 0; i<4; i++)
		{
			tatya_pl[i].ri_ti_flag = tatya[i].ri_ti_flag;
			tatya_pl[i].fu_ro_suu = tatya[i].fu_ro_suu;
		}
		tatya_pl[0].aka_fu_ro = tatya[0].aka_fu_ro;
		tatya_pl[0].aka_moti = tatya[0].aka_moti;
		tatya_pl[0].jifuu = tatya[0].jifuu;
		tatya_pl[0].ippatu = tatya[0].ippatu;
		tatya_pl[0].tenho = tatya[0].tenho;
		for (int i = 0; i<34; i++)
		{
			tatya_pl[0].hai[i] = tatya[0].hai[i];
			tatya_pl[0].genbutu[i] = tatya[0].genbutu[i];
			tatya_pl[0].tegawari[i] = tatya[0].tegawari[i];
		}

		field game_field_pl;
		game_field_pl.bafuu = game_field->bafuu;
		game_field_pl.kyoku = game_field->kyoku;
		game_field_pl.ri_ti_bou = game_field->ri_ti_bou;
		game_field_pl.game_long = game_field->game_long;
		game_field_pl.honba = game_field->honba;
		game_field_pl.nokori_maisuu = game_field->nokori_maisuu;
		game_field_pl.haitei = game_field->haitei;
		game_field_pl.kan_flag = game_field->kan_flag;
		game_field_pl.mieta_aka_hai = game_field->mieta_aka_hai;
		//for(int i=0;i<5;i++){game_field_pl.dora[i]=util.trance37to34(game_field->dora[i]);}
		for (int i = 0; i<5; i++) { game_field_pl.dora[i] = game_field->dora[i]; }

		for (int i = 0; i<4; i++) { tatya_pl[0].fu_ro[i] = tatya[0].fu_ro[i]; }
		for (int i = 0; i<tatya[0].sutehai.size(); i++) { tatya_pl[0].sutehai.push_back(tatya[0].sutehai[i]); }
		//for(int i=0;i<tatya[0].tedasi.size();i++){tatya_pl[0].tedasi.push_back(tatya[0].tedasi[i]);}

		//util.hai_check(mieteruhai);
		//cout<<endl;
		//cout<<tenpai_pro[0]<<" "<<tenpai_pro[1]<<" "<<tenpai_pro[2]<<" "<<endl;
		//cout<<tatya[1].ri_ti_flag<<" "<<tatya[2].ri_ti_flag<<" "<<tatya[3].ri_ti_flag<<" "<<endl;
		//cout<<tokuten_pro[util.IntToString(1)+"_"+util.IntToString(4)]<<" "<<tokuten_pro[util.IntToString(2)+"_"+util.IntToString(4)]<<" "<<tokuten_pro[util.IntToString(3)+"_"+util.IntToString(4)]<<endl;

		int kouho = 0;

		if (tenpai_pro[0]<sim_threshold && tenpai_pro[1]<sim_threshold && tenpai_pro[2]<sim_threshold)
		{
			kouho = op_move2(tatya, game_field, mieteruhai, mv_rank);
		}
		else
		{
			vector<double> mv_val;
			vector<int> mv;

			monte(tatya_pl, &game_field_pl, mieteruhai, pass_pro, tktn, tenpai_pro, &gouhoushu1, &exp1, &tumorare1, 1);
			double max_value = -100000.0;

			for (int k = 0; k<(signed)gouhoushu1.size() - 1; k++)//降りを除
			{
				double value = 0.0;
				value += exp1[k];
				if (gouhoushu1[k]<68)
				{
					for (int pl = 1; pl<4; pl++)
					{
						//cout<<pl<<" "<<tatya[pl].ippatu<<" "<<(gouhoushu[k])<<" "<<tenpai_pro[pl-1]<<" "<<matiyomi_pro[util.IntToString(pl)+"_"+util.IntToString(gouhoushu[k])]<<" "<<tokuten_pro[util.IntToString(pl)+"_"+util.IntToString(gouhoushu[k])]<<endl;
						value -= tenpai_pro[pl - 1] * matiyomi_pro[util.IntToString(pl) + "_" + util.IntToString(gouhoushu1[k])] * tokuten_pro[util.IntToString(pl) + "_" + util.IntToString(gouhoushu1[k])];
					}
				}
				for (int pl = 1; pl<4; pl++)
				{
					//cout<<pl<<" "<<tokuten_pro[util.IntToString(pl)+"_"+util.IntToString(-1)]<<endl;
					value -= tumorare1[k + gouhoushu1.size()*(pl - 1)] * tokuten_pro_tsumo[util.IntToString(pl) + "_" + util.IntToString(-1)];
				}
				//cout<<gouhoushu1[k]<<" "<<value<<" "<<exp1[k]<<" "<<exp1[k]-value<<endl;
				mv.push_back(gouhoushu1[k]);
				mv_val.push_back(value);

				if (max_value<value)
				{
					max_value = value;
					kouho = gouhoushu1[k];
				}
			}


			for (int i = 0; i<mv.size(); i++)
			{
				for (int j = 0; j<mv.size() - 1; j++)
				{
					if (mv_val[j]>mv_val[j + 1])
					{
						int tmp = mv[j];
						double tmp_val = mv_val[j];
						mv[j] = mv[j + 1];
						mv_val[j] = mv_val[j + 1];
						mv[j + 1] = tmp;
						mv_val[j + 1] = tmp_val;
					}
				}
			}
			mv_rank->clear();
			for (int i = mv.size() - 1; i >= 0; i--)
			{
				//cout<<mv[i]<<" "<<mv_val[i]<<endl;
				mv_rank->push_back(mv[i]);
			}
		}
		//while(1){}
		return kouho;
	}
	int move_naki(int hai[], int mae_kind, int naki_hai[], player tatya[], field *game_field, int dare, int aka, int *out)
	{
		vector<int> gouhoushu;
		vector<double> exp;
		vector<double> tumorare;

		int sutehai[34] = { 0 };
		for (int i = 0; i<37; i++)
		{
			sutehai[util.trance37to34(i)] += game_field->sutehai_sarasi[i];
		}
		int mieteruhai[34] = { 0 };
		make_mieteruhai(0, tatya, mieteruhai, sutehai);

		monte_naki(tatya, game_field, mae_kind, dare, aka, &gouhoushu, &exp, &tumorare);

		double tenpai_pro[3] = { 0.0 };
		boost::unordered_map<string, double> matiyomi_pro;
		boost::unordered_map<string, double> tokuten_pro;
		boost::unordered_map<string, double> tokuten_pro_tsumo;

		//cal_tenpai_pro(tenpai_pro, tenpai_wei, 0, tatya, game_field, sutehai);
		matiyomi_pro = cal_matiyomi_pro( matiyomi_wei, 0, tatya, game_field, mieteruhai);
		cal_tokuten_pro(tokuten_pro, tokuten_wei, 0, tatya, game_field, mieteruhai);

		//int cp_flag[3]={0};
		//for(int i=1;i<4;i++){cp_flag[i-1]=tatya[i].ri_ti_flag;}
		//for(int i=1;i<4;i++){if(tatya[i].fu_ro_suu==0){tatya[i].ri_ti_flag=1;}}
		cal_tokuten_pro(tokuten_pro_tsumo, tokuten_wei, 0, tatya, game_field, mieteruhai);
		//for(int i=1;i<4;i++){tatya[i].ri_ti_flag=cp_flag[i-1];}

		int kouho = 0;
		int naki_kouho = 0;
		double max_value = -100000.0;

		for (int k = 0; k<(signed)gouhoushu.size(); k++)
		{
			int kouho_aa = gouhoushu[k] % 100;
			int naki_kouho_aa = gouhoushu[k] / 100;

			double value = 0.0;
			value += exp[k];
			if (naki_kouho_aa != 0)
			{
				for (int pl = 1; pl<4; pl++)
				{
					//cout<<pl<<" "<<(gouhoushu[k])<<" "<<tokuten_pro[util.IntToString(pl)+"_"+util.IntToString(gouhoushu[k])]<<endl;
					value -= tenpai_pro[pl - 1] * matiyomi_pro[util.IntToString(pl) + "_" + util.IntToString(kouho_aa)] * tokuten_pro[util.IntToString(pl) + "_" + util.IntToString(kouho_aa)];
				}
			}
			for (int pl = 1; pl<4; pl++)
			{
				//cout<<pl<<" "<<tokuten_pro[util.IntToString(pl)+"_"+util.IntToString(-1)]<<endl;
				value -= tumorare[k + gouhoushu.size()*(pl - 1)] * tokuten_pro_tsumo[util.IntToString(pl) + "_" + util.IntToString(-1)];
			}
			cout << gouhoushu[k] << " " << value << " " << exp[k] << " " << exp[k] - value << endl;

			if (max_value<value)
			{
				max_value = value;
				naki_kouho = naki_kouho_aa;
				kouho = kouho_aa;
			}
		}
		//鳴くかどうか		
		if (naki_kouho == 0) { return 0; }
		else
		{
			*out = kouho;
			//鳴く牌を選ぶ
			printf("nakuyo %d kiru %d\n", naki_kouho, kouho);
			if (naki_kouho == 1)
			{
				naki_hai[0] = util.trance34to37(mae_kind);
				if (mae_kind<27 && mae_kind % 9 == 4 && tatya[0].hai[util.trance34to37(mae_kind) + 1] == 1)
				{
					naki_hai[1] = util.trance34to37(mae_kind) + 1;
				}
				else { naki_hai[1] = util.trance34to37(mae_kind); }
			}
			else if (naki_kouho == 2)
			{
				naki_hai[0] = util.trance34to37(mae_kind);
				if (mae_kind<27 && mae_kind % 9 == 4 && tatya[0].hai[util.trance34to37(mae_kind) + 1] == 1)
				{
					naki_hai[1] = util.trance34to37(mae_kind) + 1;
				}
				else { naki_hai[1] = util.trance34to37(mae_kind); }
				naki_hai[2] = util.trance34to37(mae_kind);
			}
			else if (naki_kouho == 3)
			{
				if (mae_kind % 9 == 6)
				{
					if (tatya[0].hai[util.trance34to37(mae_kind) - 2] == 1)
					{
						naki_hai[0] = util.trance34to37(mae_kind) - 2;
					}
					else { naki_hai[0] = util.trance34to37(mae_kind) - 3; }
					naki_hai[1] = util.trance34to37(mae_kind) - 1;
				}
				else if (mae_kind % 9 == 5)
				{
					naki_hai[0] = util.trance34to37(mae_kind) - 3;
					if (tatya[0].hai[util.trance34to37(mae_kind) - 1] == 1)
					{
						naki_hai[1] = util.trance34to37(mae_kind) - 1;
					}
					else { naki_hai[1] = util.trance34to37(mae_kind) - 2; }
				}
				else
				{
					naki_hai[0] = util.trance34to37(mae_kind) - 2;
					naki_hai[1] = util.trance34to37(mae_kind) - 1;
				}
			}
			else if (naki_kouho == 4)
			{
				if (mae_kind % 9 == 3)
				{
					naki_hai[0] = util.trance34to37(mae_kind) - 1;
					if (tatya[0].hai[util.trance34to37(mae_kind) + 2] == 1)
					{
						naki_hai[1] = util.trance34to37(mae_kind) + 2;
					}
					else { naki_hai[1] = util.trance34to37(mae_kind) + 1; }
				}
				else if (mae_kind % 9 == 4)
				{
					naki_hai[0] = util.trance34to37(mae_kind) - 1;
					naki_hai[1] = util.trance34to37(mae_kind) + 2;
				}
				else if (mae_kind % 9 == 5)
				{
					if (tatya[0].hai[util.trance34to37(mae_kind) - 1] == 1)
					{
						naki_hai[0] = util.trance34to37(mae_kind) - 1;
					}
					else { naki_hai[0] = util.trance34to37(mae_kind) - 2; }
					naki_hai[1] = util.trance34to37(mae_kind) + 1;
				}
				else
				{
					naki_hai[0] = util.trance34to37(mae_kind) - 1;
					naki_hai[1] = util.trance34to37(mae_kind) + 1;
				}
			}
			else if (naki_kouho == 5)
			{
				if (mae_kind % 9 == 2)
				{
					naki_hai[0] = util.trance34to37(mae_kind) + 1;
					if (tatya[0].hai[util.trance34to37(mae_kind) + 3] == 1)
					{
						naki_hai[1] = util.trance34to37(mae_kind) + 3;
					}
					else { naki_hai[1] = util.trance34to37(mae_kind) + 2; }
				}
				else if (mae_kind % 9 == 3)
				{
					if (tatya[0].hai[util.trance34to37(mae_kind) + 2] == 1)
					{
						naki_hai[0] = util.trance34to37(mae_kind) + 2;
					}
					else { naki_hai[0] = util.trance34to37(mae_kind) + 1; }
					naki_hai[1] = util.trance34to37(mae_kind) + 3;
				}
				else if (mae_kind % 9 == 4)
				{
					naki_hai[0] = util.trance34to37(mae_kind) + 2;
					naki_hai[1] = util.trance34to37(mae_kind) + 3;
				}
				else
				{
					naki_hai[0] = util.trance34to37(mae_kind) + 1;
					naki_hai[1] = util.trance34to37(mae_kind) + 2;
				}
			}

			return 1;
		}
		return 0;
	}

	int op_move(player tatya[], field *game_field, int mieteruhai[])
	{
		double max_value = -1000000000.0, value = 0.0;
		int player = 0;
		int kouho = 0;
		int aka_moti_copy = 0;
		aka_moti_copy = tatya[player].aka_moti;
		for (int k = 0; k<34; k++) { if (tatya[player].hai[k]>0) { kouho = k; break; } }
		for (int k = 0; k<34; k++)
		{
			if (tatya[player].hai[k]>0)
			{
				tatya[player].hai[k]--;
				aka_moti_copy = cut_aka(tatya[player].hai, k, tatya[player].aka_moti);
				tatya[player].aka_moti -= aka_moti_copy;

				value = make_feature(tatya, game_field, mieteruhai);
				tatya[player].hai[k]++;

				tatya[player].aka_moti += aka_moti_copy;
				//cout<<k<<" "<<value<<endl;
				//value=cal_score(&feature);
				if (value>max_value)
				{
					max_value = value;
					kouho = k;
				}
			}
		}
		//�ÃJ�����\��
		for (int l = 0; l<34; l++)
		{
			if (tatya[player].hai[l] == 4)
			{
				tatya[player].hai[l] = 0;
				tatya[player].fu_ro[tatya[player].fu_ro_suu] = 102 + l;
				tatya[player].fu_ro_suu++;
				value = make_feature(tatya, game_field, mieteruhai);
				tatya[player].hai[l] = 4;
				tatya[player].fu_ro_suu--;
				tatya[player].fu_ro[tatya[player].fu_ro_suu] = -1;
				//value=cal_score(&feature);
				if (max_value<value)
				{
					max_value = value;
					kouho = l + 102;
				}
			}
		}
		//���J��
		for (int l = 0; l<4; l++)
		{
			if (tatya[player].fu_ro[l] >= 34 && tatya[player].hai[tatya[player].fu_ro[l] % 34]>0)
			{
				int k = tatya[player].fu_ro[l] % 34;
				tatya[0].hai[k]--;
				tatya[0].fu_ro[l] += 34;
				value = make_feature(tatya, game_field, mieteruhai);
				tatya[0].hai[k]++;
				tatya[0].fu_ro[l] -= 34;
				if (max_value<value)
				{
					max_value = value;
					kouho = k + 68;
				}
			}
		}

		return kouho;
	}
	int op_move2(player tatya[], field *game_field, int mieteruhai[], vector<int> *mv_rank)
	{
		vector<double> mv_val;
		vector<int> mv;
		double max_value = -1000000000.0, value = 0.0;
		int player = 0;
		int kouho = 0;
		int aka_moti_copy = 0;
		aka_moti_copy = tatya[player].aka_moti;
		for (int k = 0; k<34; k++) { if (tatya[player].hai[k]>0) { kouho = k; break; } }
		for (int k = 0; k<34; k++)
		{
			if (tatya[player].hai[k]>0)
			{
				tatya[player].hai[k]--;
				aka_moti_copy = cut_aka(tatya[player].hai, k, tatya[player].aka_moti);
				tatya[player].aka_moti -= aka_moti_copy;

				value = make_feature(tatya, game_field, mieteruhai);
				tatya[player].hai[k]++;

				tatya[player].aka_moti += aka_moti_copy;
				//cout<<k<<" "<<value<<endl;
				//value=cal_score(&feature);
				mv.push_back(k);
				mv_val.push_back(value);
				if (value>max_value)
				{
					max_value = value;
					kouho = k;
				}
			}
		}
		//�ÃJ�����\��
		for (int l = 0; l<34; l++)
		{
			if (tatya[player].hai[l] == 4)
			{
				tatya[player].hai[l] = 0;
				tatya[player].fu_ro[tatya[player].fu_ro_suu] = 102 + l;
				tatya[player].fu_ro_suu++;
				value = make_feature(tatya, game_field, mieteruhai);
				tatya[player].hai[l] = 4;
				tatya[player].fu_ro_suu--;
				tatya[player].fu_ro[tatya[player].fu_ro_suu] = -1;
				//value=cal_score(&feature);
				mv.push_back(l + 102);
				mv_val.push_back(value);
				if (max_value<value)
				{
					max_value = value;
					kouho = l + 102;
				}
			}
		}
		//���J��
		for (int l = 0; l<4; l++)
		{
			if (tatya[player].fu_ro[l] >= 34 && tatya[player].hai[tatya[player].fu_ro[l] % 34]>0)
			{
				int k = tatya[player].fu_ro[l] % 34;
				tatya[0].hai[k]--;
				tatya[0].fu_ro[l] += 34;
				value = make_feature(tatya, game_field, mieteruhai);
				tatya[0].hai[k]++;
				tatya[0].fu_ro[l] -= 34;
				mv.push_back(k + 68);
				mv_val.push_back(value);
				if (max_value<value)
				{
					max_value = value;
					kouho = k + 68;
				}
			}
		}
		for (int i = 0; i<mv.size(); i++)
		{
			for (int j = 0; j<mv.size() - 1; j++)
			{
				if (mv_val[j]>mv_val[j + 1])
				{
					int tmp = mv[j];
					double tmp_val = mv_val[j];
					mv[j] = mv[j + 1];
					mv_val[j] = mv_val[j + 1];
					mv[j + 1] = tmp;
					mv_val[j + 1] = tmp_val;
				}
			}
		}
		mv_rank->clear();
		for (int i = mv.size() - 1; i >= 0; i--)
		{
			//cout<<mv[i]<<" "<<mv_val[i]<<endl;
			mv_rank->push_back(mv[i]);
		}
		return kouho;
	}
	int op_move_naki(player tatya[], field *game_field, int mieteruhai[], int mae_kind, int dare, int aka)
	{
		vector<int> dora_hyou;
		for (int i = 0; i<5; i++)
		{
			if (game_field->dora[i] == -1) { break; }
			dora_hyou.push_back(util.trance37to34(game_field->dora[i]));
		}
		double max_value = 0.0, value = 0.0;

		vector<int> naki_pattern;

		int kouho = 0;
		int naki_kouho = 0;

		max_value = make_feature(tatya, game_field, mieteruhai);

		//util.hai_check(hai);
		//cout<<result_paifu[i]<<endl;

		int hai_copy[34] = { 0 };
		int fu_ro_copy[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int aka_moti_copy = tatya[0].aka_moti;
		for (int i = 0; i<34; i++) { hai_copy[i] = tatya[0].hai[i]; }
		for (int i = 0; i<4; i++) { fu_ro_copy[i] = tatya[0].fu_ro[i]; }

		naki_check_push_pattern(tatya[0].hai, 0, dare, mae_kind, &naki_pattern);
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		if (aka>0) { aka_fu_ro_copy++; }

		tatya[0].fu_ro_suu++;
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			tatya[0].aka_moti = aka_moti_copy;

			hai_naki_nuku(hai_copy, tatya[0].hai, fu_ro_copy, tatya[0].fu_ro, mae_kind, naki_pattern[can_naki]);

			if (naki_pattern[can_naki] == 2)
			{
				value = make_feature(tatya, game_field, mieteruhai);
				if (max_value<value)
				{
					max_value = value;
					kouho = 0;
					naki_kouho = 2;
				}
			}
			else
			{
				for (int k = 0; k<34; k++)
				{
					if (tatya[0].hai[k]>0 && mae_kind != k)
					{
						if (!(mae_kind - 3 == k && naki_pattern[can_naki] == 3) && !(mae_kind + 3 == k && naki_pattern[can_naki] == 5))
						{
							int ctak = cut_aka(tatya[0].hai, k, tatya[0].aka_moti);
							tatya[0].aka_moti -= ctak;
							tatya[0].hai[k]--;
							//util.hai_check(hai_copy);
							value = make_feature(tatya, game_field, mieteruhai);
							tatya[0].hai[k]++;
							tatya[0].aka_moti += ctak;
							//{printf("%d %d %lf %lf\n",kouho,k,max_value,value);}
							if (max_value<value)
							{
								max_value = value;
								kouho = k;
								naki_kouho = naki_pattern[can_naki];
							}
						}
					}
				}//for(k=0;k<34;k++)
			}//if(naki_pattern[can_naki]==2)else
		}//for(int can_naki=0;can_naki<naki_pattern.size();can_naki++)
		 //cout<<100*naki_kouho+kouho<<endl;

		for (int i = 0; i<34; i++) { tatya[0].hai[i] = hai_copy[i]; }
		for (int i = 0; i<4; i++) { tatya[0].fu_ro[i] = fu_ro_copy[i]; }
		tatya[0].aka_moti = aka_moti_copy;
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		return 100 * naki_kouho + kouho;
	}
	void make_tumo_rand(player tatya[], field *game_field, int mieteruhai[], vector<int> *yama, vector<int> *sute, vector<int> *aka_yama, vector<double> *ra, vector<int> *ura, int sp[], double tenpai[3][18], double fold[3][18])
	{
		/*util.randam(0);
		util.genrand_real1();
		util.randam(0);
		util.genrand_real1();*/
		yama->clear();
		sute->clear();
		aka_yama->clear();
		ura->clear();
		ra->clear();
		int i = 0, j, l = 0;
		int tmp = 0, tmp_aka = 0;
		int haikakuritu[136] = { 0 };
		int aka_hai[136] = { 0 };
		int cp_akamie = 0;//game_field->mieta_aka_hai;
		for (i = 0; i<34; i++)
		{
			j = 4 - mieteruhai[i];
			for (int ii = 0; ii<j; ii++)
			{
				haikakuritu[l] = i;
				int aka = 0;
				if (i % 9 == 4 && i<27)
				{
					if (i == 4 && cp_akamie % 2 == 0)
					{
						aka = 1; cp_akamie += 1;
					}
					else if (i == 13 && cp_akamie % 4 <= 1)
					{
						aka = 2; cp_akamie += 2;
					}
					else if (i == 22 && cp_akamie / 4 == 0)
					{
						aka = 4; cp_akamie += 4;
					}
				}
				aka_hai[l] = aka;
				l++;
			}
		}


		for (int i = 0; i<l; i++)
		{
			j = (util.randam(0) & 0x0fffffff) % (l - i);
			tmp = haikakuritu[j + i];
			tmp_aka = aka_hai[j + i];
			haikakuritu[j + i] = haikakuritu[i];
			aka_hai[j + i] = aka_hai[i];
			haikakuritu[i] = tmp;
			aka_hai[i] = tmp_aka;
		}

		for (int i = 0; i<game_field->nokori_maisuu; i++)
		{
			//cout<<haikakuritu[i]<<" ";
			yama->push_back(haikakuritu[i]);
			sute->push_back(haikakuritu[i]);
			aka_yama->push_back(aka_hai[i]);
		}

		//cout<<endl;

		for (int i = 0; i<5; i++)
		{
			if (game_field->dora[i] != -1) { ura->push_back(haikakuritu[game_field->nokori_maisuu + i]); }
		}
		for (int i = 0; i<5 * game_field->nokori_maisuu + 6; i++)
		{
			ra->push_back(util.genrand_real1());
		}
		/*int mie[34]={0};
		for(int i=0;i<34;i++){mie[i]=mieteruhai[i];}
		for(int i=3;i<game_field->nokori_maisuu;i+=4){mie[haikakuritu[i]]++;}

		double all=0.0;
		for(int i=0;i<34;i++){all+=sutehai_pro[game_field->dora[0]][mie[i]][i];}
		int max=33;
		for(int ii=33;ii==0;ii--){if(mie[ii]!=4){max=ii;break;}}
		for(int i=0;i<game_field->nokori_maisuu;i++)
		{
		double sute_dec=util.genrand_real1()*all;
		double sute_tmp=0.0;
		for(int j=0;j<=max;j++)
		{
		sute_tmp+=sutehai_pro[game_field->dora[0]][mie[j]][j];
		//cout<<j<<" "<<sute_tmp<<" "<<sute_dec<<" "<<all<<endl;
		if(sute_tmp>sute_dec || j==max)
		{
		//cout<<j<<" ";
		sute->push_back(j);
		all+=sutehai_pro[game_field->dora[0]][mie[j]+1][j]-sutehai_pro[game_field->dora[0]][mie[j]][j];
		mie[j]++;
		for(int ii=max;ii==0;ii--){if(mie[ii]!=4){max=ii;break;}}
		j=34;
		}
		}
		}*/
		for (int i = 0; i<3; i++)
		{
			if (tatya[i + 1].fu_ro_suu != 0) { sp[i] = 1; }
			else if (tatya[i + 1].ri_ti_flag >= 1) { sp[i] = 0; }
			else { sp[i] = (util.genrand_real1()>0.60); }
		}

		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<18; j++)
			{
				fold[i][j] = util.genrand_real1();
				tenpai[i][j] = util.genrand_real1();
			}
		}

		return;
	}

	double one_turn_expect(int move, player tatya[], field *game_field, int mieteruhai[], int sp[], int tenpai[], int fold[], double pass_pro[3][34], double tktn[9][35], vector<int> ura, double fv[], double fv_pr[])
	{
		if (move >= 34) { return 10000.0; }
		if (tenpai[0] + tenpai[1] + tenpai[2] == 0 || fold[0] + fold[1] + fold[2] == 3) { return 10000.0; }
		double state_pro = 1.0;
		double val = 0.0;
		double win[3] = { 0.0 };
		double tile_pro[34] = { 0.0 };

		int sum = 0;
		for (int i = 0; i<34; i++) { sum += 4 - mieteruhai[i]; }
		for (int i = 0; i<34; i++) { tile_pro[i] = (double)(4 - mieteruhai[i]) / (double)sum; }

		double my_win_val = 0.0;
		double my_win_pass = 0.0;
		int ukeire[34] = { 0 };
		int titoi_ukeire[34] = { 0 };
		int kokusi_ukeire[34] = { 0 };
		int sha = ukeire_bit(tatya[0].hai, tatya[0].fu_ro_suu, ukeire);
		int titoi_sha = util.titoi_shanten(tatya[0].hai, tatya[0].fu_ro_suu);
		int kokusi_sha = util.kokusi_shanten(tatya[0].hai, tatya[0].fu_ro_suu);

		if (titoi_sha == 0)
		{
			for (int i = 0; i<34; i++)
			{
				if (tatya[0].hai[i] == 1) { titoi_ukeire[i] = 1; }
			}
		}
		if (kokusi_sha == 0)
		{
			if (tatya[0].hai[0] * tatya[0].hai[8] * tatya[0].hai[9] * tatya[0].hai[17] * tatya[0].hai[18] * tatya[0].hai[26] * tatya[0].hai[27] * tatya[0].hai[28] * tatya[0].hai[29] * tatya[0].hai[30] * tatya[0].hai[31] * tatya[0].hai[32] * tatya[0].hai[33] == 1)
			{
				kokusi_ukeire[0] = 1; kokusi_ukeire[8] = 1; kokusi_ukeire[9] = 1; kokusi_ukeire[17] = 1; kokusi_ukeire[18] = 1; kokusi_ukeire[26] = 1;
				kokusi_ukeire[27] = 1; kokusi_ukeire[28] = 1; kokusi_ukeire[29] = 1; kokusi_ukeire[30] = 1; kokusi_ukeire[31] = 1; kokusi_ukeire[32] = 1; kokusi_ukeire[33] = 1;
			}
			else
			{
				for (int i = 0; i <= 2; i++)
				{
					for (int j = 0; j <= 1; j++) { if (tatya[0].hai[9 * i + 8 * j] == 0) { kokusi_ukeire[9 * i + 8 * j] = 1; } }
				}
				for (int i = 27; i<34; i++)
				{
					if (tatya[0].hai[i] == 0) { kokusi_ukeire[i] = 1; }
				}
			}
		}
		if (sha == 0 || titoi_sha == 0 || kokusi_sha == 0)
		{
			int yakuari = 0;
			for (int k = 0; k<34; k++)
			{
				if (ukeire[k] == 1 || titoi_ukeire[k] == 1 || kokusi_ukeire[k] == 1)
				{
					int han_fu[2] = { 0 };
					tatya[0].hai[k]++;
					int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, k, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
					//cout<<k<<" "<<ten<<" "<<tile_pro[k]<<endl;
					my_win_val += (double)(ten + game_field->ri_ti_bou * 10)*(4 - mieteruhai[k]);
					yakuari += (4 - mieteruhai[k]);
					my_win_pass += tile_pro[k];
					tatya[0].hai[k]--;
				}
			}
			if (yakuari>0) { my_win_val /= (double)yakuari; }
		}
		int frtn = 0;
		for (int k = 0; k<34; k++)
		{
			if ((ukeire[k] == 1 || titoi_ukeire[k] == 1 || kokusi_ukeire[k] == 1) && tatya[0].genbutu[k] == 1) { frtn = 1; }
		}
		//cout<<"my "<<my_win_val<<" "<<my_win_pass<<endl;
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<34; j++)
			{
				win[i] += tile_pro[j] * (1.0 - pass_pro[i][j]);
			}
		}
		//cout<<"win "<<win[0]<<" "<<win[1]<<" "<<win[2]<<endl;
		for (int i = 0; i<3; i++) { state_pro *= (1.0 - tenpai[i] * (1 - fold[i])*(1.0 - pass_pro[i][move])); }//jibun no pai ga tooru

		for (int i = 0; i<min(3, game_field->nokori_maisuu); i++)
		{
			//cout<<"turn "<<i<<" "<<state_pro<<" "<<val<<endl;
			val -= state_pro*tenpai[i] * win[i] * tktn[i + (tenpai[i] * (1 - sp[i]) + tatya[i].ippatu) * 3][34];//i tumo
			state_pro *= (1.0 - tenpai[i] * win[i]);//i agaranai
			val += state_pro*(1 - fold[i])*(1 - frtn)*my_win_pass*my_win_val;//jibun agari
			state_pro *= (1.0 - my_win_pass*(1 - fold[i])*(1 - frtn));//jibun agarenai
			state_pro *= (1.0 - tenpai[(i + 1) % 3] * win[(i + 1) % 3] * (1 - fold[i]))*(1.0 - tenpai[(i + 2) % 3] * win[(i + 2) % 3] * (1 - fold[i]));//futari agarenai
		}
		//cout<<"turn "<<" "<<state_pro<<" "<<val<<endl;
		if (game_field->nokori_maisuu >= 3) { val += state_pro*my_win_val*my_win_pass; }
		else
		{
			double ten = 0.0;
			if (sha == 0 || titoi_sha == 0 || kokusi_sha == 0)
			{
				if (tenpai[0] + tenpai[1] + tenpai[2] == 0) { ten = 30.0; }
				else if (tenpai[0] + tenpai[1] + tenpai[2] == 1) { ten = 15.0; }
				else if (tenpai[0] + tenpai[1] + tenpai[2] == 2) { ten = 10.0; }
			}
			else
			{
				if (tenpai[0] + tenpai[1] + tenpai[2] == 3) { ten = -30.0; }
				else if (tenpai[0] + tenpai[1] + tenpai[2] == 2) { ten = -15.0; }
				else if (tenpai[0] + tenpai[1] + tenpai[2] == 1) { ten = -10.0; }
			}
			val += state_pro*ten;
		}
		//val=seme
		//fold_one ori
		double fold_one = 0.0;
		state_pro = 1.0;
		for (int i = 0; i<min(3, game_field->nokori_maisuu); i++)
		{
			//cout<<"turn "<<i<<" "<<state_pro<<" "<<val<<endl;
			fold_one -= state_pro*tenpai[i] * win[i] * tktn[i + (tenpai[i] * (1 - sp[i]) + tatya[i].ippatu) * 3][34];//i tumo
			state_pro *= (1.0 - tenpai[i] * win[i]);//i agaran
			state_pro *= (1.0 - tenpai[(i + 1) % 3] * win[(i + 1) % 3] * (1 - fold[i]))*(1.0 - tenpai[(i + 2) % 3] * win[(i + 2) % 3] * (1 - fold[i]));//futari agarenai
		}
		//cout<<"turn "<<" "<<state_pro<<" "<<val<<endl;

		int jun = 0;
		for (int i = 0; i<17; i++) { if (fv_pr[i] == fv_pr[i + 1]) { jun = i; i = 18; } }

		if (game_field->nokori_maisuu >= 3) { fold_one += state_pro*(fv_pr[jun - game_field->nokori_maisuu / 4] - fv_pr[jun + 1 - game_field->nokori_maisuu / 4]) / fv_pr[jun] * fv[jun]; }
		else
		{
			double ten = 0.0;
			if (tenpai[0] + tenpai[1] + tenpai[2] == 3) { ten = -30.0; }
			else if (tenpai[0] + tenpai[1] + tenpai[2] == 2) { ten = -15.0; }
			else if (tenpai[0] + tenpai[1] + tenpai[2] == 1) { ten = -10.0; }

			fold_one += state_pro*ten;
		}

		return val - fold_one;
	}
	double game_sim(int pl, player tatya[], field *game_field, int mieteruhai[], int ankan, double pass_pro[3][34], double tktn[9][35], double tenpai_pro[], double tenpai_add[2][18], double fv[], double fv_pr[], vector<int> *yama, vector<double> *ra, vector<int> ura, vector<int> *aka_yama, int sp[3], double fold[3][18], double tenpai_ary[3][18], int fold_flag[3], int tenpai[3], int *reult_type)
	{
		int player = pl;
		int tumoban[4] = { 0 };
		tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

		double tmp_ri_ti = 0.0;
		for (int j = 0; j<(signed)yama->size(); j++)
		{
			vector<int>::iterator yama_it = yama->begin();
			vector<int>::iterator aka_yama_it = aka_yama->begin();
			vector<double>::iterator ra_it = ra->begin();
			//cout<<"jun"<<j<<" "<<yama[j]<<" "<<yama.size()<<endl;
			game_field->nokori_maisuu--;
			int jun = 0;
			if (tumoban[player]<(signed)yama->size())
			{
				jun = tumoban[player];
				tumoban[player] += 4;
			}
			else
			{
				jun = 100;
				for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
				tumoban[player] = jun + 4;
			}
			yama_it += jun;
			aka_yama_it += jun;

			if (*yama_it % 9 != 4 && *aka_yama_it>0) { cout << "err" << *yama_it << " " << *aka_yama_it << endl; while (1) {} }
			if (player != 0)
			{
				tatya[player].ippatu = 0;
				int sim_game_jun = (signed)tatya[player].sutehai.size() + j / 4;
				if (sim_game_jun>17) { sim_game_jun = 17; }
				if (fold_flag[player - 1] == 0 && tenpai[player - 1] == 0 && tenpai_add[sp[player - 1]][sim_game_jun]>tenpai_ary[player - 1][j / 4])
				{
					tenpai[player - 1] = 1;
					if (sp[player - 1] == 0)
					{
						game_field->ri_ti_bou++;
						tatya[player].ippatu = 1;
					}
				}
				if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
				else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }
			}
			//cout<<"fo"<<fold_flag[0]<<fold_flag[1]<<fold_flag[2]<<endl;
			//cout<<"te"<<tenpai[0]<<tenpai[1]<<tenpai[2]<<endl;

			//if(17-game_field->nokori_maisuu/4>=18 ||17-game_field->nokori_maisuu/4<0){cout<<"error"<<endl;while(1){}}

			if (player == 0)//�c��
			{
				tatya[0].hai[*yama_it]++;
				tatya[0].aka_moti += *aka_yama_it;
				mieteruhai[*yama_it]++;


				//util.hai_check(tatya[0].hai);
				int kk = 0;
				for (int ii = 0; ii<34; ii++)
				{
					kk += tatya[0].hai[ii];
				}
				kk += 3 * tatya[0].fu_ro_suu;
				if (kk != 14)
				{
					cout << "error" << " " << jun << " " << *yama_it << " " << tatya[0].fu_ro_suu << endl; util.hai_check(tatya[0].hai);
					cout << tatya[0].fu_ro[0] << " " << tatya[0].fu_ro[1] << " " << tatya[0].fu_ro[2] << " " << tatya[0].fu_ro[3] << endl;
					while (1) {}
				}

				if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1)
				{
					int han_fu[2] = { 0 };
					int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, *yama_it, 1, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
					if (ten>0)
					{
						*reult_type = 0;//agari

						return ten + game_field->ri_ti_bou * 10 - tmp_ri_ti;
					}
				}
				if (tatya[0].ri_ti_flag == 1)
				{
					tatya[0].hai[*yama_it]--;
					tatya[0].aka_moti -= *aka_yama_it;
					tatya[0].ippatu = 0;
					int furikomi = 4;
					for (int ii = 0; ii<3; ii++)
					{
						ra_it = ra->begin() + 5 * jun + ii;
						if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][*yama_it]<*ra_it)//�U�荞��
						{
							furikomi = ii; ii = 3;
						}
					}
					if (furikomi != 4)
					{
						*reult_type = 1;//frkm
						return -tktn[furikomi + ((1 - sp[furikomi]) + tatya[furikomi + 1].ippatu) * 3][*yama_it] - tmp_ri_ti;
					}
				}
				else
				{
					int te = op_move(tatya, game_field, mieteruhai);
					//cout<<"te"<<te<<endl;
					if (te >= 102)
					{
						tatya[0].fu_ro[tatya[0].fu_ro_suu] = te;
						tatya[0].fu_ro_suu++;
						ankan++;
						tatya[0].hai[te % 34] = 0;
					}
					else if (te >= 68)
					{
						for (int fu = 0; fu<tatya[0].fu_ro_suu; fu++)
						{
							if (tatya[0].fu_ro[fu] == 34 + te % 34) { tatya[0].fu_ro[fu] += 34; }
						}
						tatya[0].hai[te % 34]--;
						player = 3;
					}
					else
					{
						tatya[0].hai[te]--;
						tatya[0].genbutu[te] = 1;
						if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
						else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
						else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

						int furikomi = 4;
						double ev = 0.0;
						for (int ii = 0; ii<3; ii++)
						{
							ra_it = ra->begin() + 5 * jun + ii;
							if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<*ra_it)//�U�荞��
							{
								furikomi = ii;
							}
							//if(tenpai[ii]==1 &&fold_flag[ii]==0){cout<<"pas "<<te<<" "<<pass_pro[ii][te]<<" "<<tktn[ii+3*sp[ii]][te]<<endl;}
							ev -= tenpai[ii] * (1 - fold_flag[ii])*(1.0 - pass_pro[ii][te])*tktn[ii + ((1 - sp[ii]) + tatya[ii + 1].ippatu) * 3][te];
						}

						if (one_turn_expect(te, tatya, game_field, mieteruhai, sp, tenpai, fold_flag, pass_pro, tktn, ura, fv, fv_pr) + ev<0.0)
						{
							//cout<<j<<" "<<ev<<" "<<fv[j/4]<<endl;
							*reult_type = 5;//fold
							return fv[j / 4] - tmp_ri_ti;
						}
						else if (furikomi != 4)
						{
							*reult_type = 1;//frkm
							return -tktn[furikomi + ((1 - sp[furikomi]) + tatya[furikomi + 1].ippatu) * 3][te] - tmp_ri_ti;
						}
						else if (tatya[0].fu_ro_suu - ankan == 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0)
						{
							tatya[0].ri_ti_flag = 1;
							tmp_ri_ti = 10.0;
							game_field->ri_ti_bou++;
							tatya[0].ippatu = 1;
						}
					}
				}
			}//if(player==0)
			else if (fold_flag[player - 1] == 0)
			{
				//cout<<17-game_field->nokori_maisuu/4<<" "<<pass_pro[player-1][17-game_field->nokori_maisuu/4][yama[jun]]<<endl;
				ra_it = ra->begin() + 5 * jun + 1;
				if (tenpai[player - 1] == 1 && pass_pro[player - 1][*yama_it]<*ra_it)//�c��
				{
					*reult_type = 2;//tmrr
					return -tktn[player - 1 + ((1 - sp[player - 1]) + tatya[player].ippatu) * 3][34] - tmp_ri_ti;
				}
				else
				{
					int dis_pi = 0;
					dis_pi = *yama_it;
					mieteruhai[dis_pi]++;
					tatya[0].hai[dis_pi]++;
					tatya[0].aka_moti += *aka_yama_it;

					if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1 && frtn(tatya[0].hai, tatya[0].genbutu, dis_pi, tatya[0].fu_ro_suu) == 0)
					{
						int han_fu[2] = { 0 };
						int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, dis_pi, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
						if (ten>0)
						{
							*reult_type = 0;//agari
							return ten + game_field->ri_ti_bou * 10 - tmp_ri_ti;
						}
					}
					tatya[0].hai[dis_pi]--;
					tatya[0].aka_moti -= *aka_yama_it;
					int pl1 = 0, pl2 = 0;
					if (player == 1) { pl1 = 2; pl2 = 3; }
					else if (player == 2) { pl1 = 1; pl2 = 3; }
					else if (player == 3) { pl1 = 1; pl2 = 2; }

					ra_it = ra->begin() + 5 * jun + 2;
					if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<*ra_it)//���ړ�
					{
						*reult_type = 3;//yoko
						return -tmp_ri_ti;
					}
					else
					{
						if (tatya[0].ri_ti_flag == 0 && util.check_nakeru(tatya[0].hai, dis_pi, player) == 1)
						{
							int te = op_move_naki(tatya, game_field, mieteruhai, dis_pi, player, 0);
							//cout<<"nte"<<te<<endl;
							if (te != 0)
							{
								int naki = te / 100;
								te = te % 100;

								if (naki == 1)//�|��
								{
									tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 34;
									tatya[0].hai[dis_pi] -= 2;
									tatya[0].hai[te]--;
									tatya[0].genbutu[te] = 1;
								}
								else if (naki == 2)//�J��
								{
									tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 68;
									tatya[0].hai[dis_pi] = 0;
								}
								else//�`�[
								{
									tatya[0].hai[te]--;
									tatya[0].genbutu[te] = 1;
									if (naki == 3)
									{
										tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 2;
										tatya[0].hai[dis_pi - 1]--;
										tatya[0].hai[dis_pi - 2]--;
									}
									else if (naki == 4)
									{
										tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 1;
										tatya[0].hai[dis_pi - 1]--;
										tatya[0].hai[dis_pi + 1]--;
									}
									else if (naki == 5)
									{
										tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi;
										tatya[0].hai[dis_pi + 1]--;
										tatya[0].hai[dis_pi + 2]--;
									}
								}

								int furikomi = 4;
								double ev = 0.0;
								for (int ii = 0; ii<3; ii++)
								{
									ra_it = ra->begin() + 5 * jun + ii + 3;
									if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][te]<*ra_it)//�U�荞��
									{
										furikomi = ii;
									}
									if (te<34) {
										//	cout<<te<<"ii"<<ii<<"sp"<<sp[ii]<<"ipp"<<tatya[ii+1].ippatu<<endl;
										ev -= tenpai[ii] * (1 - fold_flag[ii])*(1.0 - pass_pro[ii][te])*tktn[ii + ((1 - sp[ii]) + tatya[ii + 1].ippatu) * 3][te];
									}
								}
								if (one_turn_expect(te, tatya, game_field, mieteruhai, sp, tenpai, fold_flag, pass_pro, tktn, ura, fv, fv_pr) + ev<0.0)
								{
									*reult_type = 5;//fold
									return fv[j / 4] - tmp_ri_ti;
								}
								else if (furikomi != 4)
								{
									*reult_type = 1;//frkm
									return -tktn[furikomi + ((1 - sp[furikomi]) + tatya[furikomi + 1].ippatu) * 3][te] - tmp_ri_ti;
								}
								tatya[0].fu_ro_suu++;
								if (naki != 2)
								{
									player = 0;
									if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
									else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
									else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
								}
								else { player = 3; }
								for (int ii = 0; ii<4; ii++) { tatya[ii].ippatu = 0; }
							}//if(te!=0)
						}//if(tatya[0].ri_ti_flag==0 && util.check_nakeru(tatya[0].hai,yama[j],player)==1)
					}//���ړ�
				}//�c��
			}//���邩
			player = (player + 1) % 4;
		}//for(int j=0;j<turn;j++)

		int tmpai = 0;
		if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0) { tmpai = 1; }
		double ten = 0.0;
		if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 30.0; }
		else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = 15.0; }
		else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = 10.0; }
		else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 0) { ten = 0.0; }
		else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 0.0; }
		else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = -10.0; }
		else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = -15.0; }
		else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 0) { ten = -30.0; }
		*reult_type = 4;//ryuukyoku
		return ten + game_field->ri_ti_bou * 10 / 4 - tmp_ri_ti;
	}

	int monte(player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[6][34], double tenpai_pro[], vector<int> *kouho, vector<double> *exp, vector<double> *tumorare, int thread_num)
	{
		vector<int> yama;
		vector<int> sute;
		vector<double> ra;
		vector<int> ura;
		vector<int> aka_yama;
		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<int> nanka;
		vector<double> cc;
		vector<double> dd;

		vector<int> ri_ti_num;
		vector<double> ryu_num;
		kouho->clear();
		exp->clear();
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i]>0)
			{
				kouho->push_back(i);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i] == 4)
			{
				kouho->push_back(i + 102);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<4; i++)
		{
			if (tatya[0].fu_ro[i] >= 34 && tatya[0].hai[tatya[0].fu_ro[i] % 34]>0)
			{
				kouho->push_back(tatya[0].fu_ro[i] % 34 + 68);
				exp->push_back(0.0);
			}
		}

		kouho->push_back(-1);//�~���p
		exp->push_back(0.0);


		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			cc.push_back(0.0);
			dd.push_back(0.0);
			frkm.push_back(0);
			yoko.push_back(0);
			nanka.push_back(0);
			ri_ti_num.push_back(0);
			ryu_num.push_back(0.0);
		}

		for (int i = 0; i<(signed)(3 * exp->size()); i++)
		{
			tumorare->push_back(0.0);
		}

		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };

		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int cp_genbutu[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu = tatya[0].ippatu;
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			cp_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		int sim_num = 0;

		clock_t start;
		start = clock();

		while (clock() - start <= simtime)
			//while(sim_num<100)
		{
			sim_num++;
			/*cout<<endl;
			cout<<clock()-start<<" "<<sim_num;*/
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };

			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);
			for (int i = 0; i<(signed)kouho->size(); i++)
			{
				int player = 1;
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				//cout<<*kouho_it<<" ";

				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_pro[ii]>tenpai_ary[ii][0]) { tenpai[ii] = 1; }
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; copy_mieteruhai[j] = mieteruhai[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
				tatya[0].fu_ro_suu = cp_fu_ro_suu;
				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				tatya[0].ippatu = cp_ippatu;
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				tatya[0].aka_moti = aka_moti;

				game_field->ri_ti_bou = cp_ribou;
				game_field->nokori_maisuu = cp_nokori;

				ankan = cp_ankan;
				int flag = 0;
				if (*kouho_it >= 0)//�~��Ă��Ȃ�
				{
					if (*kouho_it<34)
					{
						tatya[0].hai[*kouho_it]--;
						tatya[0].genbutu[*kouho_it] = 1;
						if (*kouho_it == 4 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
						else if (*kouho_it == 13 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
						else if (*kouho_it == 22 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

						for (int ii = 0; ii<3; ii++)
						{
							if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][*kouho_it]<ra[ii]) { flag = 1; }
						}
					}
					else if (*kouho_it >= 102)
					{
						tatya[0].hai[*kouho_it % 34] = 0;
						tatya[0].fu_ro[tatya[0].fu_ro_suu] = *kouho_it;
						tatya[0].fu_ro_suu++;
						ankan++;
					}
					else if (*kouho_it >= 68)
					{
						for (int l = 0; l<4; l++)
						{
							if (tatya[0].fu_ro[l] >= 34 && tatya[0].hai[tatya[0].fu_ro[l] % 34]>0)
							{
								tatya[0].hai[*kouho_it % 34] = 0;
								tatya[0].fu_ro[l] += 34;
							}
						}
					}
				}//if(*kouho_it!=-1)
				if (flag == 0)
				{
					int tumoban[4] = { 0 };
					tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

					for (int j = 0; j<(signed)yama.size(); j++)
					{
						//cout<<"jun"<<j<<" "<<yama[j]<<" "<<yama.size()<<endl;
						game_field->nokori_maisuu--;
						int jun = 0;
						if (tumoban[player]<(signed)yama.size())
						{
							jun = tumoban[player];
							tumoban[player] += 4;
						}
						else
						{
							jun = 100;
							for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
							tumoban[player] = jun + 4;
						}
						if (player != 0)
						{
							int sim_game_jun = (signed)tatya[player].sutehai.size() + j / 4;
							if (sim_game_jun>17) { sim_game_jun = 17; }
							if (fold_flag[player - 1] == 0 && tenpai[player - 1] == 0 && tenpai_add[sp[player - 1]][sim_game_jun]>tenpai_ary[player - 1][j / 4])
							{
								tenpai[player - 1] = 1;
								if (sp[player - 1] == 0) { game_field->ri_ti_bou++; }
							}
							if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
							else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }
						}
						//cout<<"fo"<<fold_flag[0]<<fold_flag[1]<<fold_flag[2]<<endl;
						//cout<<"te"<<tenpai[0]<<tenpai[1]<<tenpai[2]<<endl;

						//if(17-game_field->nokori_maisuu/4>=18 ||17-game_field->nokori_maisuu/4<0){cout<<"error"<<endl;while(1){}}

						if (player == 0)//�c��
						{
							if (*kouho_it >= 0)
							{
								tatya[0].hai[yama[jun]]++;
								tatya[0].aka_moti += aka_yama[jun];
								copy_mieteruhai[yama[jun]]++;
								//util.hai_check(tatya[0].hai);
								int kk = 0;
								for (int ii = 0; ii<34; ii++)
								{
									kk += tatya[0].hai[ii];
								}
								kk += 3 * tatya[0].fu_ro_suu;
								if (kk != 14)
								{
									cout << "error" << *kouho_it << " " << jun << " " << yama[jun] << " " << tatya[0].fu_ro_suu << endl; util.hai_check(tatya[0].hai);
									cout << tatya[0].fu_ro[0] << " " << tatya[0].fu_ro[1] << " " << tatya[0].fu_ro[2] << " " << tatya[0].fu_ro[3] << endl;
									while (1) {}
								}

								if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1)
								{
									int han_fu[2] = { 0 };
									int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, yama[jun], 1, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
									if (ten>0)
									{
										/*
										if(game_field->kyoku==1 && *kouho_it==30){
										cout<<"tsumo"<<ten<<endl;
										util.hai_check(tatya[0].hai);
										cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
										cout<<j<<" "<<*kouho_it<<endl;}*/
										agari[i]++;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it += ten + game_field->ri_ti_bou * 10;
										j = (signed)yama.size();
										flag = 1;
										break;
									}
								}
								if (tatya[0].ri_ti_flag == 1)
								{
									tatya[0].hai[yama[jun]]--;
									tatya[0].ippatu = 0;
									int furikomi = 4;
									for (int ii = 0; ii<3; ii++)
									{
										if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][yama[jun]]<ra[5 * jun + ii])//�U�荞��
										{
											furikomi = ii; ii = 3;
										}
									}
									if (furikomi != 4)
									{
										dd[i] -= tktn[furikomi + 3 * sp[furikomi]][yama[jun]];
										j = (signed)yama.size(); flag = 1;
										frkm[i]++;
										//dd[i]=0.0;
									}
								}
								else
								{
									int te = op_move(tatya, game_field, copy_mieteruhai);
									//cout<<"te"<<te<<endl;
									if (te >= 102)
									{
										tatya[0].fu_ro[tatya[0].fu_ro_suu] = te;
										tatya[0].fu_ro_suu++;
										ankan++;
										tatya[0].hai[te % 34] = 0;
									}
									else if (te >= 68)
									{
										for (int fu = 0; fu<tatya[0].fu_ro_suu; fu++)
										{
											if (tatya[0].fu_ro[fu] == 34 + te % 34) { tatya[0].fu_ro[fu] += 34; }
										}
										tatya[0].hai[te % 34]--;
										player = 3;
									}
									else
									{
										tatya[0].hai[te]--;
										tatya[0].genbutu[te] = 1;
										if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
										else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
										else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

										int furikomi = 4;
										for (int ii = 0; ii<3; ii++)
										{
											if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<ra[5 * jun + ii])//�U�荞��
											{
												furikomi = ii; ii = 3;
											}
										}
										if (furikomi != 4)
										{
											dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
											j = (signed)yama.size(); flag = 1; frkm[i]++;
											//dd[i]=0.0;
										}
										else if (tatya[0].fu_ro_suu - ankan == 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0)
										{
											tatya[0].ri_ti_flag = 1;
											vector<double>::iterator exp_it = exp->begin();
											exp_it += i;
											*exp_it -= 10;
											game_field->ri_ti_bou++;
											ri_ti_num[i]++;
											nanka[i] += j;
											tatya[0].ippatu = 1;
										}
									}
								}
							}//if(*kouho_it!=-1)
						}//if(player==0)
						else if (fold_flag[player - 1] == 0)
						{
							//cout<<17-game_field->nokori_maisuu/4<<" "<<pass_pro[player-1][17-game_field->nokori_maisuu/4][yama[jun]]<<endl;

							if (tenpai[player - 1] == 1 && pass_pro[player - 1][yama[jun]]<ra[5 * jun + 1])//�c��
							{
								tmrr[i]++;

								vector<double>::iterator tumorare_it = tumorare->begin();
								tumorare_it += i + (player - 1)*exp->size();
								*tumorare_it += 1.0;
								j = (signed)yama.size(); flag = 1;
							}
							else
							{
								int dis_pi = 0;
								/*
								if(tatya[player].ri_ti_flag==0){dis_pi=sute[jun];}
								else{dis_pi=yama[jun];}*/
								dis_pi = yama[jun];

								copy_mieteruhai[dis_pi]++;
								tatya[0].hai[dis_pi]++;
								tatya[0].aka_moti += aka_yama[jun];
								//if(game_field->kyoku==1 && *kouho_it==3)util.hai_check(tatya[0].hai);

								if (*kouho_it >= 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1 && frtn(tatya[0].hai, tatya[0].genbutu, dis_pi, tatya[0].fu_ro_suu) == 0)
								{
									int han_fu[2] = { 0 };
									int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, dis_pi, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
									if (ten>0)
									{
										/*if(game_field->kyoku==1 && *kouho_it==30)
										{
										cout<<"ron"<<ten<<"aka"<<get_number_of_aka(tatya[0].aka_moti)<<endl;
										util.hai_check(tatya[0].hai);
										cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
										cout<<j<<" "<<*kouho_it<<endl;
										}*/
										agari[i]++;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it += ten + game_field->ri_ti_bou * 10;
										j = (signed)yama.size();
										flag = 1;
										break;
									}
								}
								tatya[0].hai[dis_pi]--;
								tatya[0].aka_moti -= aka_yama[jun];
								int pl1 = 0, pl2 = 0;
								if (player == 1) { pl1 = 2; pl2 = 3; }
								else if (player == 2) { pl1 = 1; pl2 = 3; }
								else if (player == 3) { pl1 = 1; pl2 = 2; }

								if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<ra[5 * jun + 2])//���ړ�
								{
									j = (signed)yama.size();
									flag = 1;
									yoko[i]++;
								}
								else
								{
									if (*kouho_it >= 0 && tatya[0].ri_ti_flag == 0 && util.check_nakeru(tatya[0].hai, dis_pi, player) == 1)
									{
										int te = op_move_naki(tatya, game_field, copy_mieteruhai, dis_pi, player, 0);
										//cout<<"nte"<<te<<endl;
										if (te != 0)
										{
											int naki = te / 100;
											te = te % 100;

											if (naki == 1)//�|��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 34;
												tatya[0].hai[dis_pi] -= 2;
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
											}
											else if (naki == 2)//�J��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 68;
												tatya[0].hai[dis_pi] = 0;
											}
											else//�`�[
											{
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
												if (naki == 3)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 2;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi - 2]--;
												}
												else if (naki == 4)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 1;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi + 1]--;
												}
												else if (naki == 5)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi;
													tatya[0].hai[dis_pi + 1]--;
													tatya[0].hai[dis_pi + 2]--;
												}
											}

											int furikomi = 4;
											for (int ii = 0; ii<3; ii++)
											{
												if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][te]<ra[5 * jun + ii + 3])//�U�荞��
												{
													furikomi = ii; ii = 3;
												}
											}
											if (furikomi != 4)
											{
												dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
												//	dd[i]=0.0;
												j = (signed)yama.size(); flag = 1; frkm[i]++;
											}
											tatya[0].fu_ro_suu++;
											if (naki != 2)
											{
												player = 0;
												if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
												else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
												else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
											}
											else { player = 3; }
										}//if(te!=0)
									}//if(tatya[0].ri_ti_flag==0 && util.check_nakeru(tatya[0].hai,yama[j],player)==1)
								}//���ړ�
							}//�c��
						}//���邩
						player = (player + 1) % 4;
					}//for(int j=0;j<turn;j++)

					if (flag == 0)
					{
						double ten = 0.0;
						if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 30.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = 15.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = 10.0; }
						else { ten = 0.0; }
						if (*kouho_it<0 || util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) != 0)
						{
							ten -= 30.0;
						}
						vector<double>::iterator exp_it = exp->begin();
						exp_it += i;
						*exp_it += game_field->ri_ti_bou * 10 / 4;

						cc[i] += ten;
						ryu_num[i] += 1.0;
					}
				}//if(flag==0)
			}//for(int i=0;i<kouho->size();i++)

			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		tatya[0].ippatu = cp_ippatu;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		tatya[0].aka_moti = aka_moti;
		game_field->ri_ti_bou = cp_ribou;

		if (thread_num == 0)
		{
			//Sleep(thread_num*1000);
			cout << "sim" << sim_num << "th" << thread_num << endl;
			for (int i = 0; i<(signed)agari.size(); i++)
			{
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << ryu_num[i] / sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)cc[i] / (double)sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)dd[i] / (double)sim_num << " " << (double)nanka[i] / (double)ri_ti_num[i] << endl;
			}
		}
		int i = 0;
		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			if (*kouho_it + cc[i] + dd[i]<cc[(signed)kouho->size() - 1])
			{
				*kouho_it = cc[(signed)kouho->size() - 1] / sim_num;
				vector<double>::iterator tumorare_it = tumorare->begin();
				vector<double>::iterator tumorare_fold = tumorare->end();
				tumorare_it += i; *tumorare_it = (*tumorare_fold / sim_num);
				tumorare_it += (signed)exp->size(); *tumorare_it = (*tumorare_fold / sim_num);
				tumorare_it += (signed)exp->size(); *tumorare_it = (*tumorare_fold / sim_num);
			}
			else
			{
				*kouho_it = (*kouho_it + cc[i] + dd[i]) / sim_num;
				vector<double>::iterator tumorare_it = tumorare->begin();
				tumorare_it += i; *tumorare_it /= sim_num;
				tumorare_it += (signed)exp->size(); *tumorare_it /= sim_num;
				tumorare_it += (signed)exp->size(); *tumorare_it /= sim_num;
			}
			i++;
		}
		i = 0;
		return sim_num;
	}
	int monte3(player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[6][35], double tenpai_pro[], vector<int> *kouho, vector<double> *exp, double fv[], int thread_num)
	{
		vector<int> yama;
		vector<int> sute;
		vector<double> ra;
		vector<int> ura;
		vector<int> aka_yama;
		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<int> fold_num;

		vector<int> ri_ti_num;
		vector<double> ryu_num;
		kouho->clear();
		exp->clear();
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i]>0)
			{
				kouho->push_back(i);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i] == 4)
			{
				kouho->push_back(i + 102);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<4; i++)
		{
			if (tatya[0].fu_ro[i] >= 34 && tatya[0].hai[tatya[0].fu_ro[i] % 34]>0)
			{
				kouho->push_back(tatya[0].fu_ro[i] % 34 + 68);
				exp->push_back(0.0);
			}
		}

		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			frkm.push_back(0);
			yoko.push_back(0);
			fold_num.push_back(0);
			ri_ti_num.push_back(0);
			ryu_num.push_back(0.0);
		}

		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };

		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int cp_genbutu[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu = tatya[0].ippatu;
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			cp_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		int sim_num = 0;

		clock_t start;
		start = clock();

		while (clock() - start <= simtime)
		{
			sim_num++;
			/*cout<<endl;
			cout<<clock()-start<<" "<<sim_num;*/
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };

			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);
			for (int i = 0; i<(signed)kouho->size(); i++)
			{
				int player = 1;
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				//cout<<*kouho_it<<" ";

				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_pro[ii]>tenpai_ary[ii][0]) { tenpai[ii] = 1; }
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; copy_mieteruhai[j] = mieteruhai[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
				tatya[0].fu_ro_suu = cp_fu_ro_suu;
				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				tatya[0].ippatu = cp_ippatu;
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				tatya[0].aka_moti = aka_moti;

				game_field->ri_ti_bou = cp_ribou;
				game_field->nokori_maisuu = cp_nokori;

				ankan = cp_ankan;
				int flag = 0;
				if (*kouho_it >= 0)//
				{
					if (*kouho_it<34)
					{
						tatya[0].hai[*kouho_it]--;
						tatya[0].genbutu[*kouho_it] = 1;
						if (*kouho_it == 4 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
						else if (*kouho_it == 13 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
						else if (*kouho_it == 22 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
					}
					else if (*kouho_it >= 102)
					{
						tatya[0].hai[*kouho_it % 34] = 0;
						tatya[0].fu_ro[tatya[0].fu_ro_suu] = *kouho_it;
						tatya[0].fu_ro_suu++;
						ankan++;
					}
					else if (*kouho_it >= 68)
					{
						for (int l = 0; l<4; l++)
						{
							if (tatya[0].fu_ro[l] >= 34 && tatya[0].hai[tatya[0].fu_ro[l] % 34]>0)
							{
								tatya[0].hai[*kouho_it % 34] = 0;
								tatya[0].fu_ro[l] += 34;
							}
						}
					}
				}//if(*kouho_it!=-1)

				int tumoban[4] = { 0 };
				tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

				for (int j = 0; j<(signed)yama.size(); j++)
				{
					//cout<<"jun"<<j<<" "<<yama[j]<<" "<<yama.size()<<endl;
					game_field->nokori_maisuu--;
					int jun = 0;
					if (tumoban[player]<(signed)yama.size())
					{
						jun = tumoban[player];
						tumoban[player] += 4;
					}
					else
					{
						jun = 100;
						for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
						tumoban[player] = jun + 4;
					}
					if (player != 0)
					{
						int sim_game_jun = (signed)tatya[player].sutehai.size() + j / 4;
						if (sim_game_jun>17) { sim_game_jun = 17; }
						if (fold_flag[player - 1] == 0 && tenpai[player - 1] == 0 && tenpai_add[sp[player - 1]][sim_game_jun]>tenpai_ary[player - 1][j / 4])
						{
							tenpai[player - 1] = 1;
							if (sp[player - 1] == 0) { game_field->ri_ti_bou++; }
						}
						if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
						else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }
					}
					//cout<<"fo"<<fold_flag[0]<<fold_flag[1]<<fold_flag[2]<<endl;
					//cout<<"te"<<tenpai[0]<<tenpai[1]<<tenpai[2]<<endl;

					//if(17-game_field->nokori_maisuu/4>=18 ||17-game_field->nokori_maisuu/4<0){cout<<"error"<<endl;while(1){}}

					if (player == 0)//�c��
					{
						tatya[0].hai[yama[jun]]++;
						tatya[0].aka_moti += aka_yama[jun];
						copy_mieteruhai[yama[jun]]++;
						//util.hai_check(tatya[0].hai);
						int kk = 0;
						for (int ii = 0; ii<34; ii++)
						{
							kk += tatya[0].hai[ii];
						}
						kk += 3 * tatya[0].fu_ro_suu;
						if (kk != 14)
						{
							cout << "error" << *kouho_it << " " << jun << " " << yama[jun] << " " << tatya[0].fu_ro_suu << endl; util.hai_check(tatya[0].hai);
							cout << tatya[0].fu_ro[0] << " " << tatya[0].fu_ro[1] << " " << tatya[0].fu_ro[2] << " " << tatya[0].fu_ro[3] << endl;
							while (1) {}
						}

						if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1)
						{
							int han_fu[2] = { 0 };
							int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, yama[jun], 1, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
							if (ten>0)
							{
								/*
								if(game_field->kyoku==1 && *kouho_it==30){
								cout<<"tsumo"<<ten<<endl;
								util.hai_check(tatya[0].hai);
								cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
								cout<<j<<" "<<*kouho_it<<endl;}*/
								agari[i]++;
								vector<double>::iterator exp_it = exp->begin();
								exp_it += i;
								*exp_it += ten + game_field->ri_ti_bou * 10;
								j = (signed)yama.size();
								flag = 1;
								break;
							}
						}
						if (tatya[0].ri_ti_flag == 1)
						{
							tatya[0].hai[yama[jun]]--;
							tatya[0].ippatu = 0;
							int furikomi = 4;
							for (int ii = 0; ii<3; ii++)
							{
								if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][yama[jun]]<ra[5 * jun + ii])//�U�荞��
								{
									furikomi = ii; ii = 3;
								}
							}
							if (furikomi != 4)
							{
								vector<double>::iterator exp_it = exp->begin();
								exp_it += i;
								*exp_it -= tktn[furikomi + 3 * sp[furikomi]][yama[jun]];
								j = (signed)yama.size(); flag = 1;
								frkm[i]++;
							}
						}
						else
						{
							int te = op_move(tatya, game_field, copy_mieteruhai);
							//cout<<"te"<<te<<endl;
							if (te >= 102)
							{
								tatya[0].fu_ro[tatya[0].fu_ro_suu] = te;
								tatya[0].fu_ro_suu++;
								ankan++;
								tatya[0].hai[te % 34] = 0;
							}
							else if (te >= 68)
							{
								for (int fu = 0; fu<tatya[0].fu_ro_suu; fu++)
								{
									if (tatya[0].fu_ro[fu] == 34 + te % 34) { tatya[0].fu_ro[fu] += 34; }
								}
								tatya[0].hai[te % 34]--;
								player = 3;
							}
							else
							{
								tatya[0].hai[te]--;
								tatya[0].genbutu[te] = 1;
								if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
								else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
								else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

								int furikomi = 4;
								double ev = 0.0;
								for (int ii = 0; ii<3; ii++)
								{
									if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<ra[5 * jun + ii])//�U�荞��
									{
										furikomi = ii;
									}
									ev -= tenpai[ii] * (1 - fold_flag[ii])*pass_pro[ii][te] * tktn[ii + 3 * sp[ii]][te];
								}
								if (ev<fv[j / 4])
								{
									//cout<<"out"<<ev<<" "<<fv<<endl;
									vector<double>::iterator exp_it = exp->begin();
									exp_it += i;
									*exp_it += fv[j / 4];
									fold_num[i]++;
									j = (signed)yama.size(); flag = 1;
								}
								else if (furikomi != 4)
								{
									vector<double>::iterator exp_it = exp->begin();
									exp_it += i;
									*exp_it -= tktn[furikomi + 3 * sp[furikomi]][te];
									j = (signed)yama.size(); flag = 1; frkm[i]++;
								}
								else if (tatya[0].fu_ro_suu - ankan == 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0)
								{
									tatya[0].ri_ti_flag = 1;
									vector<double>::iterator exp_it = exp->begin();
									exp_it += i;
									*exp_it -= 10;
									game_field->ri_ti_bou++;
									ri_ti_num[i]++;
									tatya[0].ippatu = 1;
								}
							}
						}
					}//if(player==0)
					else if (fold_flag[player - 1] == 0)
					{
						//cout<<17-game_field->nokori_maisuu/4<<" "<<pass_pro[player-1][17-game_field->nokori_maisuu/4][yama[jun]]<<endl;

						if (tenpai[player - 1] == 1 && pass_pro[player - 1][yama[jun]]<ra[5 * jun + 1])//�c��
						{
							tmrr[i]++;
							vector<double>::iterator exp_it = exp->begin();
							exp_it += i;
							*exp_it -= tktn[player - 1 + 3 * sp[player - 1]][34];
							j = (signed)yama.size();
							flag = 1;
						}
						else
						{
							int dis_pi = 0;
							/*
							if(tatya[player].ri_ti_flag==0){dis_pi=sute[jun];}
							else{dis_pi=yama[jun];}*/
							dis_pi = yama[jun];

							copy_mieteruhai[dis_pi]++;
							tatya[0].hai[dis_pi]++;
							tatya[0].aka_moti += aka_yama[jun];
							//if(game_field->kyoku==1 && *kouho_it==3)util.hai_check(tatya[0].hai);

							if (*kouho_it >= 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1 && frtn(tatya[0].hai, tatya[0].genbutu, dis_pi, tatya[0].fu_ro_suu) == 0)
							{
								int han_fu[2] = { 0 };
								int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, dis_pi, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
								if (ten>0)
								{
									/*if(game_field->kyoku==1 && *kouho_it==30)
									{
									cout<<"ron"<<ten<<"aka"<<get_number_of_aka(tatya[0].aka_moti)<<endl;
									util.hai_check(tatya[0].hai);
									cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
									cout<<j<<" "<<*kouho_it<<endl;
									}*/
									agari[i]++;
									vector<double>::iterator exp_it = exp->begin();
									exp_it += i;
									*exp_it += ten + game_field->ri_ti_bou * 10;
									j = (signed)yama.size();
									flag = 1;
									break;
								}
							}
							tatya[0].hai[dis_pi]--;
							tatya[0].aka_moti -= aka_yama[jun];
							int pl1 = 0, pl2 = 0;
							if (player == 1) { pl1 = 2; pl2 = 3; }
							else if (player == 2) { pl1 = 1; pl2 = 3; }
							else if (player == 3) { pl1 = 1; pl2 = 2; }

							if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<ra[5 * jun + 2])//���ړ�
							{
								j = (signed)yama.size();
								flag = 1;
								yoko[i]++;
							}
							else
							{
								if (*kouho_it >= 0 && tatya[0].ri_ti_flag == 0 && util.check_nakeru(tatya[0].hai, dis_pi, player) == 1)
								{
									int te = op_move_naki(tatya, game_field, copy_mieteruhai, dis_pi, player, 0);
									//cout<<"nte"<<te<<endl;
									if (te != 0)
									{
										int naki = te / 100;
										te = te % 100;

										if (naki == 1)//�|��
										{
											tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 34;
											tatya[0].hai[dis_pi] -= 2;
											tatya[0].hai[te]--;
											tatya[0].genbutu[te] = 1;
										}
										else if (naki == 2)//�J��
										{
											tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 68;
											tatya[0].hai[dis_pi] = 0;
										}
										else//�`�[
										{
											tatya[0].hai[te]--;
											tatya[0].genbutu[te] = 1;
											if (naki == 3)
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 2;
												tatya[0].hai[dis_pi - 1]--;
												tatya[0].hai[dis_pi - 2]--;
											}
											else if (naki == 4)
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 1;
												tatya[0].hai[dis_pi - 1]--;
												tatya[0].hai[dis_pi + 1]--;
											}
											else if (naki == 5)
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi;
												tatya[0].hai[dis_pi + 1]--;
												tatya[0].hai[dis_pi + 2]--;
											}
										}

										int furikomi = 4;
										double ev = 0.0;
										for (int ii = 0; ii<3; ii++)
										{
											if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][te]<ra[5 * jun + ii + 3])//�U�荞��
											{
												furikomi = ii;
											}
											if (te<34) { ev -= tenpai[ii] * (1 - fold_flag[ii])*pass_pro[ii][te] * tktn[ii + 3 * sp[ii]][te]; }
										}
										if (ev<fv[j / 4])
										{
											//cout<<"out"<<endl;
											vector<double>::iterator exp_it = exp->begin();
											exp_it += i;
											*exp_it += fv[j / 4]; fold_num[i]++;
											j = (signed)yama.size(); flag = 1;
										}
										else if (furikomi != 4)
										{
											vector<double>::iterator exp_it = exp->begin();
											exp_it += i;
											*exp_it -= tktn[furikomi + 3 * sp[furikomi]][te];
											j = (signed)yama.size(); flag = 1; frkm[i]++;
										}
										tatya[0].fu_ro_suu++;
										if (naki != 2)
										{
											player = 0;
											if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
											else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
											else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
										}
										else { player = 3; }
									}//if(te!=0)
								}//if(tatya[0].ri_ti_flag==0 && util.check_nakeru(tatya[0].hai,yama[j],player)==1)
							}//���ړ�
						}//�c��
					}//���邩
					player = (player + 1) % 4;
				}//for(int j=0;j<turn;j++)

				if (flag == 0)
				{
					int tmpai = 0;
					if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0) { tmpai = 1; }
					double ten = 0.0;
					if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 30.0; }
					else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = 15.0; }
					else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = 10.0; }
					else if (tmpai == 1 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 0) { ten = 0.0; }
					else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 0.0; }
					else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = -10.0; }
					else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = -15.0; }
					else if (tmpai == 0 && fold_flag[0] + fold_flag[1] + fold_flag[2] == 0) { ten = -30.0; }

					vector<double>::iterator exp_it = exp->begin();
					exp_it += i;
					*exp_it += ten + game_field->ri_ti_bou*10.0 / 4.0;

					ryu_num[i] += 1.0;
				}
			}//for(int i=0;i<kouho->size();i++)

			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		tatya[0].ippatu = cp_ippatu;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		tatya[0].aka_moti = aka_moti;
		game_field->ri_ti_bou = cp_ribou;

		if (thread_num == 0)
		{
			//Sleep(thread_num*1000);
			cout << "sim" << sim_num << "th" << thread_num << endl;
			for (int i = 0; i<(signed)agari.size(); i++)
			{
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << ryu_num[i] / sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)fold_num[i] / (double)sim_num << endl;
			}
		}
		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			*kouho_it /= sim_num;
			if (*kouho_it<fv[0])
			{
				*kouho_it = fv[0];
			}
		}
		return sim_num;
	}
	int monte4(player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[9][35], double tenpai_pro[], vector<int> *kouho, vector<double> *exp, double fv[], double fv_pr[], int thread_num)
	{
		vector<int> yama;
		vector<int> sute;
		vector<double> ra;
		vector<int> ura;
		vector<int> aka_yama;
		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<int> fold_num;

		vector<int> ri_ti_num;
		vector<int> ryu_num;
		kouho->clear();
		exp->clear();
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i]>0)
			{
				kouho->push_back(i);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<34; i++)
		{
			if (tatya[0].hai[i] == 4)
			{
				kouho->push_back(i + 102);
				exp->push_back(0.0);
			}
		}
		for (int i = 0; i<4; i++)
		{
			if (tatya[0].fu_ro[i] >= 34 && tatya[0].hai[tatya[0].fu_ro[i] % 34]>0)
			{
				kouho->push_back(tatya[0].fu_ro[i] % 34 + 68);
				exp->push_back(0.0);
			}
		}

		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			frkm.push_back(0);
			yoko.push_back(0);
			fold_num.push_back(0);
			ri_ti_num.push_back(0);
			ryu_num.push_back(0);
		}

		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };

		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int cp_genbutu[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu[4] = { 0 };


		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			cp_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		for (int j = 0; j<4; j++) { cp_ippatu[j] = tatya[j].ippatu; }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		int sim_num = 0;

		clock_t start;
		start = clock();

		while (clock() - start <= simtime)
		{
			sim_num++;
			/*cout<<endl;
			cout<<clock()-start<<" "<<sim_num;*/
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };

			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);
			for (int i = 0; i<(signed)kouho->size(); i++)
			{
				int player = 1;
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				//cout<<*kouho_it<<" ";
				game_field->ri_ti_bou = cp_ribou;
				game_field->nokori_maisuu = cp_nokori;
				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_pro[ii]>tenpai_ary[ii][0])
					{
						tenpai[ii] = 1;
						if (sp[ii] == 0)
						{
							tatya[ii + 1].ippatu = 1;
						}
					}
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; copy_mieteruhai[j] = mieteruhai[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
				tatya[0].fu_ro_suu = cp_fu_ro_suu;
				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				for (int j = 0; j<4; j++) { tatya[j].ippatu = cp_ippatu[j]; }
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				tatya[0].aka_moti = aka_moti;

				ankan = cp_ankan;

				if (*kouho_it<34)
				{
					if (*kouho_it == 4 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
					else if (*kouho_it == 13 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
					else if (*kouho_it == 22 && tatya[0].hai[*kouho_it] == 1 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
					tatya[0].hai[*kouho_it]--;
					tatya[0].genbutu[*kouho_it] = 1;
				}
				else if (*kouho_it >= 102)
				{
					tatya[0].hai[*kouho_it % 34] = 0;
					tatya[0].fu_ro[tatya[0].fu_ro_suu] = *kouho_it;
					tatya[0].fu_ro_suu++;
					ankan++;
				}
				else if (*kouho_it >= 68)
				{
					for (int l = 0; l<4; l++)
					{
						if (tatya[0].fu_ro[l] >= 34 && tatya[0].hai[tatya[0].fu_ro[l] % 34]>0)
						{
							tatya[0].hai[*kouho_it % 34] = 0;
							tatya[0].fu_ro[l] += 34;
						}
					}
				}

				if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0 && tatya[0].fu_ro_suu == 0 && tatya[0].tensuu >= 10)
				{
					tatya[0].ri_ti_flag = 1;
					tatya[0].ippatu = 1;
				}
				int result_type = 0;

				*exp_it += game_sim(player, tatya, game_field, copy_mieteruhai, cp_ankan, pass_pro, tktn, tenpai_pro, tenpai_add, fv, fv_pr, &yama, &ra, ura, &aka_yama, sp, fold, tenpai_ary, fold_flag, tenpai, &result_type);

				if (result_type == 0) { agari[i]++; }
				else if (result_type == 1) { frkm[i]++; }
				else if (result_type == 2) { tmrr[i]++; }
				else if (result_type == 3) { yoko[i]++; }
				else if (result_type == 4) { ryu_num[i]++; }
				else if (result_type == 5) { fold_num[i]++; }
			}//for(int i=0;i<kouho->size();i++)

			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = cp_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		for (int j = 0; j<4; j++) { tatya[j].ippatu = cp_ippatu[j]; }
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		tatya[0].aka_moti = aka_moti;
		game_field->ri_ti_bou = cp_ribou;

		if (thread_num == 0)
		{
			//Sleep(thread_num*1000);
			cout << "sim" << sim_num << "th" << thread_num << endl;
			for (int i = 0; i<(signed)agari.size(); i++)
			{
				vector<int>::iterator kouho_it = kouho->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << (double)ryu_num[i] / (double)sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)fold_num[i] / (double)sim_num << endl;
			}
		}

		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			*kouho_it /= sim_num;
			if (*kouho_it<fv[0])
			{
				*kouho_it = fv[0];
			}
		}
		return sim_num;
	}

	int monte_naki(player tatya[], field *game_field, int mae_kind, int dare, int aka, vector<int> *gouhoushu, vector<double> *exp, vector<double> *tumorare)
	{
		int sutehai[34] = { 0 };
		for (int i = 0; i<37; i++)
		{
			sutehai[util.trance37to34(i)] += game_field->sutehai_sarasi[i];
		}
		int mieteruhai[34] = { 0 };
		make_mieteruhai(0, tatya, mieteruhai, sutehai);

		vector<int>yama;
		vector<int>sute;
		vector<int>aka_yama;
		vector<double> ra;
		vector<int>ura;
		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<double> cc;
		vector<double> dd;

		vector<int> ri_ti_num;
		vector<double> ryu_num;
		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }


		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		double tenpai_pro[3] = { 0.0 };
		boost::unordered_map<string, double> matiyomi_pro;
		boost::unordered_map<string, double> tokuten_pro;

		//cal_tenpai_pro(tenpai_pro, tenpai_wei, 0, tatya, game_field, sutehai);
		matiyomi_pro = cal_matiyomi_pro(matiyomi_wei, 0, tatya, game_field, mieteruhai);

		cout << tenpai_pro[0] << " " << tenpai_pro[1] << " " << tenpai_pro[2] << " " << endl;

		double tktn[6][34] = { 0.0 };

		int cp_flag[3] = { 0 };
		for (int i = 1; i<4; i++) { cp_flag[i - 1] = tatya[i].ri_ti_flag; }
		for (int i = 1; i<4; i++) { if (tatya[i].fu_ro_suu == 0) { tatya[i].ri_ti_flag = 1; } }
		cal_tokuten_pro(tokuten_pro, tokuten_wei, 0, tatya, game_field, mieteruhai);
		for (int i = 1; i<4; i++) { tatya[i].ri_ti_flag = cp_flag[i - 1]; }

		for (int i = 0; i<3; i++) { for (int j = 0; j<34; j++) { tktn[i][j] = tokuten_pro[util.IntToString(i + 1) + "_" + util.IntToString(j)]; } }
		cal_tokuten_pro(tokuten_pro, tokuten_wei, 0, tatya, game_field, mieteruhai);
		for (int i = 0; i<3; i++) { for (int j = 0; j<34; j++) { tktn[i + 3][j] = tokuten_pro[util.IntToString(i + 1) + "_" + util.IntToString(j)]; } }

		double tenpai_add[2][18] = {
			{ 0.0,0.002335,0.006462,0.012885,0.021763,0.031192,0.040902,0.04881,0.055549,0.059249,0.061399,0.061758,0.060759,0.057774,0.054347,0.050596,0.045914,0.020536 },
			{ 0.0,0.004186,0.012679,0.026525,0.043686,0.061091,0.076685,0.086221,0.09071,0.089312,0.084598,0.076227,0.067402,0.057426,0.047591,0.038911,0.029836,0.01132 } };

		double tenpai_jun[3][36] = { { 0.0 } };

		for (int j = 0; j<3; j++)
		{
			for (int i = 0; i<36; i++)
			{
				if (i % 18 == 0) { tenpai_jun[j][i] = tenpai_pro[j]; }
				else if ((signed)tatya[j + 1].sutehai.size() + i % 18<18)
				{
					tenpai_jun[j][i] += tenpai_add[i >= 18][(signed)tatya[j + 1].sutehai.size() + i % 18];
					if (tenpai_jun[j][i]>1.0) { tenpai_jun[j][i] = 1.0; }
				}
				else
				{
					tenpai_jun[j][i] += tenpai_add[i >= 18][17];
					if (tenpai_jun[j][i]>1.0) { tenpai_jun[j][i] = 1.0; }
				}
				if (i % 18 != 17) { tenpai_jun[j][i + 1] = tenpai_jun[j][i]; }
			}
		}
		for (int i = 0; i<18; i++)
		{
			cout << tenpai_jun[0][i] << " ";
		}
		cout << endl;

		double pass_pro[3][34] = { { 0.0 } };
		for (int i = 0; i<34; i++)
		{
			for (int j = 0; j<3; j++)
			{
				pass_pro[j][i] = (1.0 - matiyomi_pro[util.IntToString(j + 1) + "_" + util.IntToString(i)]);
			}
		}

		vector<int> naki_pattern;
		naki_check_push_pattern(tatya[0].hai, 0, dare, mae_kind, &naki_pattern);

		gouhoushu->push_back(0);//���Ȃ�
		exp->push_back(0.0);
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			if (naki_pattern[can_naki] == 2) { gouhoushu->push_back(200); exp->push_back(0.0); }
		}

		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_genbutu[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu = tatya[0].ippatu;
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			copy_mieteruhai[j] = mieteruhai[j];
			copy_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, naki_pattern[can_naki]);
			for (int k = 0; k<34; k++)
			{
				if (tatya[0].hai[k]>0 && mae_kind != k)
				{
					if (naki_pattern[can_naki] != 2 && !(mae_kind - 3 == k && naki_pattern[can_naki] == 3) && !(mae_kind + 3 == k && naki_pattern[can_naki] == 5))
					{
						//cout<<100*naki_pattern[can_naki]+k<<endl;
						gouhoushu->push_back(100 * naki_pattern[can_naki] + k);
						exp->push_back(0.0);
					}
				}
			}
		}
		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			cc.push_back(0.0);
			dd.push_back(0.0);
			frkm.push_back(0);
			yoko.push_back(0);
			ri_ti_num.push_back(0);
			ryu_num.push_back(0.0);
		}
		for (int i = 0; i<(signed)(3 * exp->size()); i++)
		{
			tumorare->push_back(0.0);
		}

		clock_t start;
		start = clock();
		int sim_num = 0;
		while (clock() - start <= simtime)
		{
			sim_num++;
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };
			make_tumo_rand(tatya, game_field, copy_mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);

			for (int i = 0; i<(signed)gouhoushu->size(); i++)
			{
				//cout<<"i"<<i<<endl;
				int player = 1;
				vector<int>::iterator kouho_it = gouhoushu->begin();
				kouho_it += i;

				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_jun[ii][sp[ii] * 18]>tenpai_ary[ii][0]) { tenpai[ii] = 1; }
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				int nkp = *kouho_it / 100;
				int flag = 0;
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				for (int j = 0; j<34; j++)
				{
					tatya[0].hai[j] = copy[j];
					mieteruhai[j] = copy_mieteruhai[j];
					tatya[0].genbutu[j] = copy_genbutu[j];
				}
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }

				if (nkp == 0)//���Ȃ�
				{
					player = (dare + 1) % 4;
					tatya[0].fu_ro_suu = cp_fu_ro_suu;
				}
				else
				{
					hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, nkp);
					tatya[0].fu_ro_suu = cp_fu_ro_suu + 1;
					if (aka>0) { tatya[0].aka_fu_ro++; }

					if (nkp != 2)
					{
						int te = *kouho_it % 100;
						tatya[0].hai[te]--;
						tatya[0].genbutu[te] = 1;
						for (int ii = 0; ii<3; ii++)
						{
							if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<ra[ii]) { flag = 1; }
						}
					}
					player = 1;
				}
				game_field->nokori_maisuu = cp_nokori;

				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				tatya[0].ippatu = cp_ippatu;
				ankan = cp_ankan;
				tatya[0].aka_moti = aka_moti;
				game_field->ri_ti_bou = cp_ribou;
				if (flag == 0)
				{

					int tumoban[4] = { 0 };
					tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

					for (int j = 0; j<(signed)yama.size(); j++)
					{
						game_field->nokori_maisuu--;
						int jun = 0;
						if (tumoban[player]<(signed)yama.size())
						{
							jun = tumoban[player];
							tumoban[player] += 4;
						}
						else
						{
							jun = 100;
							for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
							tumoban[player] = jun + 4;
						}
						if (player != 0)
						{
							if (tenpai[player - 1] == 0 && tenpai_jun[player - 1][sp[player - 1] * 18 + j / 4]>tenpai_ary[player - 1][j / 4])
							{
								tenpai[player - 1] = 1;
								if (sp[player - 1] == 0) { game_field->ri_ti_bou++; }
							}
							if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
							else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }
						}

						//if(17-game_field->nokori_maisuu/4>=18 ||17-game_field->nokori_maisuu/4<0){cout<<"error"<<endl;while(1){}}

						//cout<<"j"<<jun<<" "<<player<<" "<<yama[jun]<<endl;
						//if(game_field->kyoku==1 && *kouho_it==3)cout<<"j"<<jun<<" "<<player<<" "<<yama[jun]<<" "<<sute[jun]<<endl;

						if (player == 0)//�c��
						{

							tatya[0].hai[yama[jun]]++;
							tatya[0].aka_moti += aka_yama[jun];
							mieteruhai[yama[jun]]++;
							//	util.hai_check(tatya[0].hai);
							int kk = 0;
							for (int ii = 0; ii<34; ii++)
							{
								kk += tatya[0].hai[ii];
							}
							kk += 3 * tatya[0].fu_ro_suu;
							if (kk != 14)
							{
								cout << "error" << *kouho_it << " " << jun << " " << yama[jun] << " " << tatya[0].fu_ro_suu << endl; util.hai_check(tatya[0].hai);
								cout << tatya[0].fu_ro[0] << " " << tatya[0].fu_ro[1] << " " << tatya[0].fu_ro[2] << " " << tatya[0].fu_ro[3] << endl;
								while (1) {}
							}

							if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1)
							{
								int han_fu[2] = { 0 };
								int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, yama[jun], 1, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
								if (ten>0)
								{
									/*if(game_field->kyoku==1 && *kouho_it==29){
									cout<<"tsumo"<<ten<<endl;
									util.hai_check(tatya[0].hai);
									cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
									cout<<j<<" "<<*kouho_it<<endl;}*/
									agari[i]++;
									vector<double>::iterator exp_it = exp->begin();
									exp_it += i;
									*exp_it += ten + game_field->ri_ti_bou * 10;
									j = (signed)yama.size();
									flag = 1;
									break;
								}
							}
							if (tatya[0].ri_ti_flag == 1)
							{
								tatya[0].hai[yama[jun]]--;
								tatya[0].ippatu = 0;
								int furikomi = 4;
								for (int ii = 0; ii<3; ii++)
								{
									if (fold_flag[ii] == 0 && pass_pro[ii][yama[jun]]<ra[5 * jun + ii])//�U�荞��
									{
										furikomi = ii; ii = 3; frkm[i]++;
									}
								}
								if (furikomi != 4)
								{
									dd[i] -= tktn[furikomi + 3 * sp[furikomi]][yama[jun]];
									j = (signed)yama.size(); flag = 1;
									frkm[i]++;
									//dd[i]=0.0;
								}
							}
							else
							{
								int te = op_move(tatya, game_field, mieteruhai);
								//cout<<"te"<<te<<endl;
								if (te >= 102)
								{
									tatya[0].fu_ro[tatya[0].fu_ro_suu] = te;
									tatya[0].fu_ro_suu++;
									ankan++;
									tatya[0].hai[te % 34] = 0;
								}
								else if (te >= 68)
								{
									for (int fu = 0; fu<tatya[0].fu_ro_suu; fu++)
									{
										if (tatya[0].fu_ro[fu] == 34 + te % 34) { tatya[0].fu_ro[fu] += 34; }
									}
									tatya[0].hai[te % 34]--;
									player = 3;
								}
								else
								{
									tatya[0].hai[te]--;
									tatya[0].genbutu[te] = 1;
									if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
									else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
									else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

									int furikomi = 4;
									for (int ii = 0; ii<3; ii++)
									{
										if (((1 - fold_flag[ii])*pass_pro[ii][te] + fold_flag[ii])<ra[5 * jun + ii])//�U�荞��
										{
											furikomi = ii; ii = 3;
										}
									}
									if (furikomi != 4)
									{
										dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
										j = (signed)yama.size(); flag = 1; frkm[i]++;
										//dd[i]=0.0;
									}
									else if (tatya[0].fu_ro_suu - ankan == 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0)
									{
										tatya[0].ri_ti_flag = 1;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it -= 10;
										game_field->ri_ti_bou++;
										tatya[0].ippatu = 1;
										ri_ti_num[i]++;
									}
								}
							}
						}
						else if (fold_flag[player - 1] == 0)
						{
							//cout<<17-game_field->nokori_maisuu/4<<" "<<pass_pro[player-1][17-game_field->nokori_maisuu/4][yama[jun]]<<endl;

							if (pass_pro[player - 1][yama[jun]]<ra[5 * jun + 1])//�c��
							{
								tmrr[i]++;

								vector<double>::iterator tumorare_it = tumorare->begin();
								tumorare_it += i + (player - 1)*exp->size();
								*tumorare_it += 1.0;
								j = (signed)yama.size(); flag = 1;
							}
							else
							{
								int dis_pi = 0;

								/*if(tatya[player].ri_ti_flag==0){dis_pi=sute[jun];}
								else{dis_pi=yama[jun];}*/
								dis_pi = yama[jun];

								mieteruhai[dis_pi]++;
								tatya[0].hai[dis_pi]++;
								tatya[0].aka_moti += aka_yama[jun];
								//if(game_field->kyoku==1 && *kouho_it==3)util.hai_check(tatya[0].hai);

								if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1 && frtn(tatya[0].hai, tatya[0].genbutu, dis_pi, tatya[0].fu_ro_suu) == 0)
								{
									int han_fu[2] = { 0 };
									int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, dis_pi, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
									if (ten>0)
									{
										/*if(game_field->kyoku==1 && *kouho_it==29)
										{
										cout<<"ron"<<ten<<"aka"<<get_number_of_aka(tatya[0].aka_moti)<<endl;
										util.hai_check(tatya[0].hai);
										cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
										cout<<j<<" "<<*kouho_it<<endl;
										}*/
										agari[i]++;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it += ten + game_field->ri_ti_bou * 10;
										j = (signed)yama.size();
										flag = 1;
										break;
									}
								}
								tatya[0].hai[dis_pi]--;
								tatya[0].aka_moti -= aka_yama[jun];
								int pl1 = 0, pl2 = 0;
								if (player == 1) { pl1 = 2; pl2 = 3; }
								else if (player == 2) { pl1 = 1; pl2 = 3; }
								else if (player == 3) { pl1 = 1; pl2 = 2; }

								if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<ra[5 * jun + 2])//���ړ�
								{
									j = (signed)yama.size();
									flag = 1;
									yoko[i]++;
								}
								else
								{
									if (tatya[0].ri_ti_flag == 0 && util.check_nakeru(tatya[0].hai, dis_pi, player) == 1)
									{
										int te = op_move_naki(tatya, game_field, mieteruhai, dis_pi, player, 0);
										//	if(game_field->kyoku==1 && *kouho_it==3)cout<<"nte"<<te<<endl;
										if (te != 0)
										{
											int naki = te / 100;
											te = te % 100;

											if (naki == 1)//�|��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 34;
												tatya[0].hai[dis_pi] -= 2;
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
											}
											else if (naki == 2)//�J��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 68;
												tatya[0].hai[dis_pi] = 0;
											}
											else//�`�[
											{
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
												if (naki == 3)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 2;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi - 2]--;
												}
												else if (naki == 4)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 1;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi + 1]--;
												}
												else if (naki == 5)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi;
													tatya[0].hai[dis_pi + 1]--;
													tatya[0].hai[dis_pi + 2]--;
												}
											}

											int furikomi = 4;
											for (int ii = 0; ii<3; ii++)
											{
												if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][te]<ra[5 * jun + ii + 3])//�U�荞��
												{
													furikomi = ii; ii = 3;
												}
											}
											if (furikomi != 4)
											{
												dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
												//dd[i]=0.0;
												j = (signed)yama.size(); flag = 1; frkm[i]++;
											}
											tatya[0].fu_ro_suu++;
											if (naki != 2)
											{
												player = 0;
												if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
												else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
												else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
											}
											else { player = 3; }
										}//if(te!=0)
									}//if(tatya[0].ri_ti_flag==0 && util.check_nakeru(tatya[0].hai,yama[j],player)==1)
								}//���ړ�
							}//�c��
						}//���邩
						player = (player + 1) % 4;
					}//for(int j=0;j<turn;j++)

					if (flag == 0)
					{
						double ten = 0.0;
						if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 30.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = 15.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = 10.0; }
						else { ten = 0.0; }
						if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) != 0)
						{
							ten -= 30.0;
						}
						vector<double>::iterator exp_it = exp->begin();
						exp_it += i;
						*exp_it += game_field->ri_ti_bou * 10 / 4.0;
						cc[i] += ten;
						ryu_num[i] += 1.0;
					}
				}//if(flag==0)
			}//for(int i=0;i<gouhoushu->size();i++)
			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		tatya[0].aka_moti = aka_moti;
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = copy_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		tatya[0].ippatu = cp_ippatu;
		game_field->ri_ti_bou = cp_ribou;
		ankan = cp_ankan;
		yama.clear();
		cout << "sim" << sim_num << endl;
		for (int i = 0; i<(signed)agari.size(); i++)
		{
			vector<int>::iterator kouho_it = gouhoushu->begin();
			kouho_it += i;
			vector<double>::iterator exp_it = exp->begin();
			exp_it += i;
			cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << ryu_num[i] / sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)cc[i] / (double)sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)dd[i] / (double)sim_num << endl;
		}
		int i = 0;
		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			if (*kouho_it + cc[i] + dd[i]<0)
			{
				*kouho_it = -15.0*ryu_num[i] / (sim_num - frkm[i] + 1);
				vector<double>::iterator tumorare_it = tumorare->begin();
				tumorare_it += i; *tumorare_it /= (sim_num - frkm[i] + 1);
				tumorare_it = tumorare->begin();
				tumorare_it += i + (signed)exp->size(); *tumorare_it /= (sim_num - frkm[i] + 1);
				tumorare_it = tumorare->begin();
				tumorare_it += i + 2 * (signed)exp->size(); *tumorare_it /= (sim_num - frkm[i] + 1);
			}
			else
			{
				*kouho_it = (*kouho_it + cc[i] + dd[i]) / sim_num;
				vector<double>::iterator tumorare_it = tumorare->begin();
				tumorare_it += i; *tumorare_it /= sim_num;
				tumorare_it = tumorare->begin();
				tumorare_it += i + (signed)exp->size(); *tumorare_it /= sim_num;
				tumorare_it = tumorare->begin();
				tumorare_it += i + 2 * (signed)exp->size(); *tumorare_it /= sim_num;
			}
			i++;
		}

		return 0;
	}
	int monte_naki2(player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[6][34], double tenpai_pro[], int mae_kind, int dare, int aka, vector<int> *gouhoushu, vector<double> *exp, vector<double> *tumorare, int thread_num)
	{
		vector<int>yama;
		vector<int>sute;
		vector<int>aka_yama;
		vector<double> ra;
		vector<int>ura;
		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<double> cc;
		vector<double> dd;

		vector<int> ri_ti_num;
		vector<double> ryu_num;
		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }

		vector<int> naki_pattern;
		naki_check_push_pattern(tatya[0].hai, 0, dare, mae_kind, &naki_pattern);

		gouhoushu->push_back(0);//���Ȃ�
		exp->push_back(0.0);
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			if (naki_pattern[can_naki] == 2) { gouhoushu->push_back(200); exp->push_back(0.0); }
		}

		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_genbutu[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu = tatya[0].ippatu;
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			copy_mieteruhai[j] = mieteruhai[j];
			copy_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, naki_pattern[can_naki]);
			for (int k = 0; k<34; k++)
			{
				if (tatya[0].hai[k]>0 && mae_kind != k)
				{
					if (naki_pattern[can_naki] != 2 && !(mae_kind - 3 == k && naki_pattern[can_naki] == 3) && !(mae_kind + 3 == k && naki_pattern[can_naki] == 5))
					{
						//cout<<100*naki_pattern[can_naki]+k<<endl;
						gouhoushu->push_back(100 * naki_pattern[can_naki] + k);
						exp->push_back(0.0);
					}
				}
			}
		}

		gouhoushu->push_back(-1);//�~��
		exp->push_back(0.0);

		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			cc.push_back(0.0);
			dd.push_back(0.0);
			frkm.push_back(0);
			yoko.push_back(0);
			ri_ti_num.push_back(0);
			ryu_num.push_back(0.0);
		}
		for (int i = 0; i<(signed)(3 * exp->size()); i++)
		{
			tumorare->push_back(0.0);
		}

		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };


		clock_t start;
		start = clock();
		int sim_num = 0;
		while (clock() - start <= simtime)
		{
			sim_num++;
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };
			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);

			for (int i = 0; i<(signed)gouhoushu->size(); i++)
			{
				//cout<<"i"<<i<<endl;
				int player = 1;
				vector<int>::iterator kouho_it = gouhoushu->begin();
				kouho_it += i;

				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_pro[ii]>tenpai_ary[ii][0]) { tenpai[ii] = 1; }
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				int nkp = *kouho_it / 100;
				int flag = 0;
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				for (int j = 0; j<34; j++)
				{
					tatya[0].hai[j] = copy[j];
					copy_mieteruhai[j] = mieteruhai[j];
					tatya[0].genbutu[j] = copy_genbutu[j];
				}
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }

				if (nkp == 0 || *kouho_it<0)//���Ȃ�
				{
					player = (dare + 1) % 4;
					tatya[0].fu_ro_suu = cp_fu_ro_suu;
				}
				else
				{
					hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, nkp);
					tatya[0].fu_ro_suu = cp_fu_ro_suu + 1;
					if (aka>0) { tatya[0].aka_fu_ro++; }

					if (nkp != 2)
					{
						int te = *kouho_it % 100;
						tatya[0].hai[te]--;
						tatya[0].genbutu[te] = 1;
						for (int ii = 0; ii<3; ii++)
						{
							if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<ra[ii]) { flag = 1; }
						}
					}
					player = 1;
				}
				game_field->nokori_maisuu = cp_nokori;

				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				tatya[0].ippatu = cp_ippatu;
				ankan = cp_ankan;
				tatya[0].aka_moti = aka_moti;
				game_field->ri_ti_bou = cp_ribou;
				if (flag == 0)
				{
					int tumoban[4] = { 0 };
					tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

					for (int j = 0; j<(signed)yama.size(); j++)
					{
						game_field->nokori_maisuu--;
						int jun = 0;
						if (tumoban[player]<(signed)yama.size())
						{
							jun = tumoban[player];
							tumoban[player] += 4;
						}
						else
						{
							jun = 100;
							for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
							tumoban[player] = jun + 4;
						}
						if (player != 0)
						{
							int sim_game_jun = (signed)tatya[player].sutehai.size() + j / 4;
							if (sim_game_jun>17) { sim_game_jun = 17; }
							if (fold_flag[player - 1] == 0 && tenpai[player - 1] == 0 && tenpai_add[sp[player - 1]][sim_game_jun]>tenpai_ary[player - 1][j / 4])
							{
								tenpai[player - 1] = 1;
								if (sp[player - 1] == 0) { game_field->ri_ti_bou++; }
							}
							if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
							else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }
						}
						//if(17-game_field->nokori_maisuu/4>=18 ||17-game_field->nokori_maisuu/4<0){cout<<"error"<<endl;while(1){}}

						/*cout<<"j"<<j<<" "<<jun<<" "<<yama.size()<<endl;
						cout<<"fo"<<fold_flag[0]<<fold_flag[1]<<fold_flag[2]<<endl;
						cout<<"te"<<tenpai[0]<<tenpai[1]<<tenpai[2]<<endl;
						*/
						//if(game_field->kyoku==1 && *kouho_it==3)cout<<"j"<<jun<<" "<<player<<" "<<yama[jun]<<" "<<sute[jun]<<endl;

						if (player == 0)//�c��
						{
							if (*kouho_it >= 0)
							{
								tatya[0].hai[yama[jun]]++;
								tatya[0].aka_moti += aka_yama[jun];
								copy_mieteruhai[yama[jun]]++;
								//	util.hai_check(tatya[0].hai);
								int kk = 0;
								for (int ii = 0; ii<34; ii++)
								{
									kk += tatya[0].hai[ii];
								}
								kk += 3 * tatya[0].fu_ro_suu;
								if (kk != 14)
								{
									cout << "error" << *kouho_it << " " << jun << " " << yama[jun] << " " << tatya[0].fu_ro_suu << endl; util.hai_check(tatya[0].hai);
									cout << tatya[0].fu_ro[0] << " " << tatya[0].fu_ro[1] << " " << tatya[0].fu_ro[2] << " " << tatya[0].fu_ro[3] << endl;
									while (1) {}
								}

								if (util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1)
								{
									int han_fu[2] = { 0 };
									int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, yama[jun], 1, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
									if (ten>0)
									{
										/*if(game_field->kyoku==1 && *kouho_it==29){
										cout<<"tsumo"<<ten<<endl;
										util.hai_check(tatya[0].hai);
										cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
										cout<<j<<" "<<*kouho_it<<endl;}*/
										agari[i]++;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it += ten + game_field->ri_ti_bou * 10;
										j = (signed)yama.size();
										flag = 1;
										break;
									}
								}
								if (tatya[0].ri_ti_flag == 1)
								{
									tatya[0].hai[yama[jun]]--;
									tatya[0].ippatu = 0;
									int furikomi = 4;
									for (int ii = 0; ii<3; ii++)
									{
										if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][yama[jun]]<ra[5 * jun + ii])//�U�荞��
										{
											furikomi = ii; ii = 3;
										}
									}
									if (furikomi != 4)
									{
										dd[i] -= tktn[furikomi + 3 * sp[furikomi]][yama[jun]];
										j = (signed)yama.size(); flag = 1;
										frkm[i]++;
										//dd[i]=0.0;
									}
								}
								else
								{
									int te = op_move(tatya, game_field, copy_mieteruhai);
									//cout<<"te"<<te<<endl;
									if (te >= 102)
									{
										tatya[0].fu_ro[tatya[0].fu_ro_suu] = te;
										tatya[0].fu_ro_suu++;
										ankan++;
										tatya[0].hai[te % 34] = 0;
									}
									else if (te >= 68)
									{
										for (int fu = 0; fu<tatya[0].fu_ro_suu; fu++)
										{
											if (tatya[0].fu_ro[fu] == 34 + te % 34) { tatya[0].fu_ro[fu] += 34; }
										}
										tatya[0].hai[te % 34]--;
										player = 3;
									}
									else
									{
										tatya[0].hai[te]--;
										tatya[0].genbutu[te] = 1;
										if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
										else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
										else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }

										int furikomi = 4;
										for (int ii = 0; ii<3; ii++)
										{
											if (tenpai[ii] == 1 && fold_flag[ii] == 0 && pass_pro[ii][te]<ra[5 * jun + ii])//�U�荞��
											{
												furikomi = ii; ii = 3;
											}
										}
										if (furikomi != 4)
										{
											dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
											j = (signed)yama.size(); flag = 1; frkm[i]++;
											//dd[i]=0.0;
										}
										else if (tatya[0].fu_ro_suu - ankan == 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == 0)
										{
											tatya[0].ri_ti_flag = 1;
											vector<double>::iterator exp_it = exp->begin();
											exp_it += i;
											*exp_it -= 10;
											game_field->ri_ti_bou++;
											tatya[0].ippatu = 1;
											ri_ti_num[i]++;
										}
									}
								}
							}//if(*kouho_it!=-1)
						}//if(player==0)//�c��
						else if (fold_flag[player - 1] == 0)
						{
							//cout<<17-game_field->nokori_maisuu/4<<" "<<pass_pro[player-1][17-game_field->nokori_maisuu/4][yama[jun]]<<endl;

							if (tenpai[player - 1] == 1 && pass_pro[player - 1][yama[jun]]<ra[5 * jun + 1])//�c��
							{
								tmrr[i]++;

								vector<double>::iterator tumorare_it = tumorare->begin();
								tumorare_it += i + (player - 1)*exp->size();
								*tumorare_it += 1.0;
								j = (signed)yama.size(); flag = 1;
							}
							else
							{
								int dis_pi = 0;

								/*if(tatya[player].ri_ti_flag==0){dis_pi=sute[jun];}
								else{dis_pi=yama[jun];}*/
								dis_pi = yama[jun];

								copy_mieteruhai[dis_pi]++;
								tatya[0].hai[dis_pi]++;
								tatya[0].aka_moti += aka_yama[jun];
								//if(game_field->kyoku==1 && *kouho_it==3)util.hai_check(tatya[0].hai);

								if (*kouho_it >= 0 && util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) == -1 && frtn(tatya[0].hai, tatya[0].genbutu, dis_pi, tatya[0].fu_ro_suu) == 0)
								{
									int han_fu[2] = { 0 };
									int ten = util.yaku(tatya[0].hai, tatya[0].fu_ro, dis_pi, 0, tatya[0].ri_ti_flag, tatya[0].ippatu, game_field->bafuu, tatya[0].jifuu, 0, game_field->dora, tatya[0].aka_fu_ro + get_number_of_aka(tatya[0].aka_moti) + get_number_of_ura_dora(tatya, ura), 0, 0, 0, han_fu);
									if (ten>0)
									{
										/*if(game_field->kyoku==1 && *kouho_it==29)
										{
										cout<<"ron"<<ten<<"aka"<<get_number_of_aka(tatya[0].aka_moti)<<endl;
										util.hai_check(tatya[0].hai);
										cout<<tatya[0].fu_ro[0]<<" "<<tatya[0].fu_ro[1]<<" "<<tatya[0].fu_ro[2]<<" "<<tatya[0].fu_ro[3]<<endl;
										cout<<j<<" "<<*kouho_it<<endl;
										}*/
										agari[i]++;
										vector<double>::iterator exp_it = exp->begin();
										exp_it += i;
										*exp_it += ten + game_field->ri_ti_bou * 10;
										j = (signed)yama.size();
										flag = 1;
										break;
									}
								}
								tatya[0].hai[dis_pi]--;
								tatya[0].aka_moti -= aka_yama[jun];
								int pl1 = 0, pl2 = 0;
								if (player == 1) { pl1 = 2; pl2 = 3; }
								else if (player == 2) { pl1 = 1; pl2 = 3; }
								else if (player == 3) { pl1 = 1; pl2 = 2; }

								if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<ra[5 * jun + 2])//���ړ�
								{
									//cout<<(tenpai[pl1-1]*pass_pro[pl1-1][dis_pi]+(1-tenpai[pl1-1]))*(tenpai[pl2-1]*pass_pro[pl2-1][dis_pi]+(1-tenpai[pl2-1]))<<" "<<ra[5*jun+2]<<endl;
									j = (signed)yama.size();
									flag = 1;
									yoko[i]++;
								}
								else
								{
									if (*kouho_it >= 0 && tatya[0].ri_ti_flag == 0 && util.check_nakeru(tatya[0].hai, dis_pi, player) == 1)
									{
										int te = op_move_naki(tatya, game_field, copy_mieteruhai, dis_pi, player, 0);
										//	if(game_field->kyoku==1 && *kouho_it==3)cout<<"nte"<<te<<endl;
										if (te != 0)
										{
											int naki = te / 100;
											te = te % 100;

											if (naki == 1)//�|��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 34;
												tatya[0].hai[dis_pi] -= 2;
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
											}
											else if (naki == 2)//�J��
											{
												tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi + 68;
												tatya[0].hai[dis_pi] = 0;
											}
											else//�`�[
											{
												tatya[0].hai[te]--;
												tatya[0].genbutu[te] = 1;
												if (naki == 3)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 2;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi - 2]--;
												}
												else if (naki == 4)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi - 1;
													tatya[0].hai[dis_pi - 1]--;
													tatya[0].hai[dis_pi + 1]--;
												}
												else if (naki == 5)
												{
													tatya[0].fu_ro[tatya[0].fu_ro_suu] = dis_pi;
													tatya[0].hai[dis_pi + 1]--;
													tatya[0].hai[dis_pi + 2]--;
												}
											}

											int furikomi = 4;
											for (int ii = 0; ii<3; ii++)
											{
												if (fold_flag[ii] == 0 && tenpai[ii] == 1 && pass_pro[ii][te]<ra[5 * jun + ii + 3])//�U�荞��
												{
													furikomi = ii; ii = 3;
												}
											}

											if (furikomi != 4)
											{
												dd[i] -= tktn[furikomi + 3 * sp[furikomi]][te];
												//dd[i]=0.0;
												j = (signed)yama.size(); flag = 1; frkm[i]++;
											}
											tatya[0].fu_ro_suu++;
											if (naki != 2)
											{
												player = 0;
												if (te == 4 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 2 == 1) { tatya[0].aka_moti--; }
												else if (te == 13 && tatya[0].hai[te] == 0 && tatya[0].aka_moti % 4 >= 2) { tatya[0].aka_moti -= 2; }
												else if (te == 22 && tatya[0].hai[te] == 0 && tatya[0].aka_moti / 4 == 1) { tatya[0].aka_moti -= 4; }
											}
											else { player = 3; }
										}//if(te!=0)
									}//if(tatya[0].ri_ti_flag==0 && util.check_nakeru(tatya[0].hai,yama[j],player)==1)
								}//���ړ�
							}//�c��
						}//���邩
						player = (player + 1) % 4;
					}//for(int j=0;j<turn;j++)

					if (flag == 0)
					{
						double ten = 0.0;
						if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 30.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = 15.0; }
						else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = 10.0; }
						else { ten = 0.0; }
						if (*kouho_it<0 || util.shantensuu_all(tatya[0].hai, tatya[0].fu_ro_suu) != 0)
						{
							ten -= 30.0;
						}
						vector<double>::iterator exp_it = exp->begin();
						exp_it += i;
						*exp_it += game_field->ri_ti_bou * 10 / 4;

						kouho_it += i;
						cc[i] += ten;
						ryu_num[i] += 1.0;
					}
				}//if(flag==0)
			}//for(int i=0;i<gouhoushu->size();i++)
			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		tatya[0].aka_moti = aka_moti;
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = copy_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		tatya[0].ippatu = cp_ippatu;
		game_field->ri_ti_bou = cp_ribou;
		ankan = cp_ankan;
		yama.clear();
		if (thread_num == 0)
		{
			cout << "sim" << sim_num << endl;

			for (int i = 0; i<(signed)agari.size(); i++)
			{
				vector<int>::iterator kouho_it = gouhoushu->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << ryu_num[i] / sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)cc[i] / (double)sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)dd[i] / (double)sim_num << endl;
			}
		}
		int i = 0;
		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			if (*kouho_it + cc[i] + dd[i]<cc[(signed)gouhoushu->size() - 1])
			{
				*kouho_it = cc[(signed)gouhoushu->size() - 1] / sim_num;
				vector<double>::iterator tumorare_it = tumorare->begin();
				vector<double>::iterator tumorare_fold = tumorare->end();
				tumorare_it += i; *tumorare_it = (*tumorare_fold / sim_num);
				tumorare_it += (signed)exp->size(); *tumorare_it = (*tumorare_fold / sim_num);
				tumorare_it += (signed)exp->size(); *tumorare_it = (*tumorare_fold / sim_num);
			}
			else
			{
				*kouho_it = (*kouho_it + cc[i] + dd[i]) / sim_num;
				vector<double>::iterator tumorare_it = tumorare->begin();
				tumorare_it += i; *tumorare_it /= sim_num;
				tumorare_it = tumorare->begin();
				tumorare_it += i + (signed)exp->size(); *tumorare_it /= sim_num;
				tumorare_it = tumorare->begin();
				tumorare_it += i + 2 * (signed)exp->size(); *tumorare_it /= sim_num;
			}
			i++;
		}

		return sim_num;
	}
	int monte_naki3(int pl, player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[9][35], double tenpai_pro[], int mae_kind, int dare, int aka, vector<int> *gouhoushu, vector<double> *exp, double fv[], double fv_pr[], int thread_num)
	{
		vector<int>yama;
		vector<int>sute;
		vector<int>aka_yama;
		vector<double> ra;
		vector<int>ura;

		vector<int> agari;
		vector<int> tmrr;
		vector<int> frkm;
		vector<int> yoko;
		vector<int> ri_ti_num;
		vector<int> ryu_num;
		vector<int> fold_num;
		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }

		vector<int> naki_pattern;
		naki_check_push_pattern(tatya[0].hai, 0, dare, mae_kind, &naki_pattern);

		gouhoushu->push_back(0);
		exp->push_back(0.0);
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			if (naki_pattern[can_naki] == 2) { gouhoushu->push_back(200); exp->push_back(0.0); }
		}

		int cp_nokori = game_field->nokori_maisuu;

		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		int copy_genbutu[34] = { 0 };
		int copy_fu_ro[4] = { 0 };
		int cp_fu_ro_suu = tatya[0].fu_ro_suu;
		int cp_ri_ti_flag = tatya[0].ri_ti_flag;
		int cp_ippatu[4] = { 0 };
		for (int j = 0; j<34; j++)
		{
			copy[j] = tatya[0].hai[j];
			copy_mieteruhai[j] = mieteruhai[j];
			copy_genbutu[j] = tatya[0].genbutu[j];
		}
		for (int j = 0; j<4; j++) { copy_fu_ro[j] = tatya[0].fu_ro[j]; }
		int ankan = 0;
		for (int i = 0; i<4; i++) { if (tatya[0].fu_ro[i] >= 102) { ankan++; } }
		for (int j = 0; j<4; j++) { cp_ippatu[j] = tatya[j].ippatu; }
		int cp_ankan = ankan;
		int aka_fu_ro_copy = tatya[0].aka_fu_ro;
		int aka_moti = tatya[0].aka_moti;

		int cp_ribou = game_field->ri_ti_bou;
		for (int can_naki = 0; can_naki<(signed)naki_pattern.size(); can_naki++)
		{
			hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, naki_pattern[can_naki]);
			for (int k = 0; k<34; k++)
			{
				if (tatya[0].hai[k]>0 && mae_kind != k)
				{
					if (naki_pattern[can_naki] != 2 && !(mae_kind - 3 == k && naki_pattern[can_naki] == 3) && !(mae_kind + 3 == k && naki_pattern[can_naki] == 5))
					{
						//cout<<100*naki_pattern[can_naki]+k<<endl;
						gouhoushu->push_back(100 * naki_pattern[can_naki] + k);
						exp->push_back(0.0);
					}
				}
			}
		}
		for (int i = 0; i<(signed)exp->size(); i++)
		{
			agari.push_back(0);
			tmrr.push_back(0);
			frkm.push_back(0);
			yoko.push_back(0);
			ri_ti_num.push_back(0);
			fold_num.push_back(0);
			ryu_num.push_back(0);
		}
		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };

		clock_t start;
		start = clock();
		int sim_num = 0;
		while (clock() - start <= simtime)
		{
			sim_num++;
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };
			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);

			for (int i = 0; i<(signed)gouhoushu->size(); i++)
			{
				//cout<<"i"<<i<<endl;

				vector<int>::iterator kouho_it = gouhoushu->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				int player = 1;
				if (*kouho_it == 0) { player = (pl + 1) % 4; }

				int fold_flag[3] = { 0 };
				int tenpai[3] = { 0 };
				for (int ii = 0; ii<3; ii++)
				{
					if (tenpai_pro[ii]>tenpai_ary[ii][0])
					{
						tenpai[ii] = 1;
						if (sp[ii] == 0)
						{
							tatya[ii + 1].ippatu = 1;
						}
					}
				}
				for (int ii = 0; ii<3; ii++)
				{
					if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
					else { fold_flag[ii] = 1; }
				}

				int nkp = *kouho_it / 100;
				tatya[0].aka_fu_ro = aka_fu_ro_copy;
				for (int j = 0; j<34; j++)
				{
					tatya[0].hai[j] = copy[j];
					copy_mieteruhai[j] = mieteruhai[j];
					tatya[0].genbutu[j] = copy_genbutu[j];
				}
				for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }

				if (nkp == 0)//
				{
					player = (dare + 1) % 4;
					tatya[0].fu_ro_suu = cp_fu_ro_suu;
				}
				else
				{
					hai_naki_nuku(copy, tatya[0].hai, copy_fu_ro, tatya[0].fu_ro, mae_kind, nkp);
					tatya[0].fu_ro_suu = cp_fu_ro_suu + 1;
					if (aka>0) { tatya[0].aka_fu_ro++; }

					if (nkp != 2)
					{
						int te = *kouho_it % 100;
						tatya[0].hai[te]--;
						tatya[0].genbutu[te] = 1;
					}
					player = 1;
				}

				int result_type = 0;
				*exp_it += game_sim(player, tatya, game_field, copy_mieteruhai, cp_ankan, pass_pro, tktn, tenpai_pro, tenpai_add, fv, fv_pr, &yama, &ra, ura, &aka_yama, sp, fold, tenpai_ary, fold_flag, tenpai, &result_type);

				if (result_type == 0) { agari[i]++; }
				else if (result_type == 1) { frkm[i]++; }
				else if (result_type == 2) { tmrr[i]++; }
				else if (result_type == 3) { yoko[i]++; }
				else if (result_type == 4) { ryu_num[i]++; }
				else if (result_type == 5) { fold_num[i]++; }

				game_field->nokori_maisuu = cp_nokori;

				tatya[0].ri_ti_flag = cp_ri_ti_flag;
				for (int j = 0; j<4; j++) { tatya[j].ippatu = cp_ippatu[j]; }
				ankan = cp_ankan;
				tatya[0].aka_moti = aka_moti;
				game_field->ri_ti_bou = cp_ribou;

			}//for(int i=0;i<gouhoushu->size();i++)
			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		game_field->nokori_maisuu = cp_nokori;

		tatya[0].aka_moti = aka_moti;
		tatya[0].fu_ro_suu = cp_fu_ro_suu;
		tatya[0].aka_fu_ro = aka_fu_ro_copy;
		for (int j = 0; j<34; j++) { tatya[0].hai[j] = copy[j]; tatya[0].genbutu[j] = copy_genbutu[j]; }
		for (int j = 0; j<4; j++) { tatya[0].fu_ro[j] = copy_fu_ro[j]; }
		tatya[0].ri_ti_flag = cp_ri_ti_flag;
		for (int j = 0; j<4; j++) { tatya[j].ippatu = cp_ippatu[j]; }
		game_field->ri_ti_bou = cp_ribou;
		ankan = cp_ankan;
		yama.clear();

		if (thread_num == 0)
		{
			//Sleep(thread_num*1000);
			cout << "sim" << sim_num << "th" << thread_num << endl;
			for (int i = 0; i<(signed)agari.size(); i++)
			{
				vector<int>::iterator kouho_it = gouhoushu->begin();
				kouho_it += i;
				vector<double>::iterator exp_it = exp->begin();
				exp_it += i;
				cout << *kouho_it << " " << (double)agari[i] / (double)sim_num << " " << (double)frkm[i] / (double)sim_num << " " << (double)tmrr[i] / (double)sim_num << " " << (double)ryu_num[i] / (double)sim_num << " " << (double)yoko[i] / (double)sim_num << " " << *exp_it / sim_num << " " << (double)ri_ti_num[i] / (double)sim_num << " " << (double)fold_num[i] / (double)sim_num << endl;
			}
		}

		int i = 0;
		for (vector<double>::iterator kouho_it = exp->begin(); kouho_it != exp->end(); kouho_it++)
		{
			*kouho_it /= sim_num;
			if (*kouho_it<fv[0])
			{
				*kouho_it = fv[0];
			}
			i++;
		}

		return sim_num;
	}

	double fold_value(int pl, player tatya[], field *game_field, int mieteruhai[], double pass_pro[3][34], double tktn[9][35], double tenpai_pro[], double fv[], double fv_pr[])
	{
		double fold_value = 0.0;
		vector<int> yama;
		vector<int> sute;
		vector<double> ra;
		vector<int> ura;
		vector<int> aka_yama;

		int tmrr = 0;
		int yoko = 0;
		int ryu_num = 0;
		int tmpi = 0;
		int fold_aa = 0;
		double tenpai_add[2][18] = {
			{ 0.000697805,0.00233589,0.00647868,0.0130019,0.0222445,0.0326084,0.0442081,0.0551696,0.0664761,0.07593,0.0851587,0.0936157,0.101705,0.107592,0.113447,0.119278,0.12424,0.128769 },
			{ 0.00089024,0.00429243,0.0131171,0.02795,0.0476165,0.0701866,0.094936,0.117831,0.140139,0.15942,0.177978,0.192965,0.208369,0.220483,0.230416,0.240306,0.244079,0.246588 } };

		if (game_field->nokori_maisuu<0) { game_field->nokori_maisuu = 0; }
		int cp_nokori = game_field->nokori_maisuu;

		int cp_ribou = game_field->ri_ti_bou;
		int sim_num[18] = { 0 };
		int cp_ippatu[3] = { 0 };

		for (int i = 1; i<4; i++)
		{
			cp_ippatu[i - 1] = tatya[i].ippatu;
		}
		clock_t start;
		start = clock();

		while (clock() - start <= simtime / 10)
		{
			int sp[3] = { 0 };
			double fold[3][18] = { { 0.0 } };
			double tenpai_ary[3][18] = { { 0.0 } };
			game_field->nokori_maisuu = cp_nokori;
			game_field->ri_ti_bou = cp_ribou;

			make_tumo_rand(tatya, game_field, mieteruhai, &yama, &sute, &aka_yama, &ra, &ura, sp, tenpai_ary, fold);

			int player = pl;

			int fold_flag[3] = { 0 };
			int tenpai[3] = { 0 };
			for (int ii = 0; ii<3; ii++)
			{
				if (tenpai_pro[ii]>tenpai_ary[ii][0])
				{
					tenpai[ii] = 1;
					if (sp[ii] == 0)
					{
						tatya[ii + 1].ippatu = 1;
					}
				}
			}
			for (int ii = 0; ii<3; ii++)
			{
				if (tatya[ii + 1].ri_ti_flag != 0 || fold[ii][0]<cal_seme_pro(ii + 1, tatya, 0, sp, tenpai)) { fold_flag[ii] = 0; }
				else { fold_flag[ii] = 1; }
			}

			int flag = 0;

			int tumoban[4] = { 0 };
			tumoban[0] = 3; tumoban[1] = 0; tumoban[2] = 1; tumoban[3] = 2;

			for (int j = 0; j<(signed)yama.size(); j++)
			{
				//cout<<"jun"<<j<<" "<<j/4<<" "<<yama.size()<<endl;
				game_field->nokori_maisuu--;
				int jun = 0;
				if (tumoban[player]<(signed)yama.size())
				{
					jun = tumoban[player];
					tumoban[player] += 4;
				}
				else
				{
					jun = 100;
					for (int ii = 0; ii<4; ii++) { if (jun>tumoban[ii]) { jun = tumoban[ii]; } }
					tumoban[player] = jun + 4;
				}
				if (player != 0)
				{
					tatya[player].ippatu = 0;
					if (fold_flag[player - 1] == 0 && tenpai[player - 1] == 0 && tenpai_add[sp[player - 1]][j / 4]>tenpai_ary[player - 1][j / 4])
					{
						tenpai[player - 1] = 1;
						if (sp[player - 1] == 0)
						{
							tatya[player].ippatu = 1;
							game_field->ri_ti_bou++;
						}
					}
					if (tatya[player].ri_ti_flag != 0 || (fold_flag[player - 1] == 0 && fold[player - 1][j / 4]<cal_seme_pro(player, tatya, j / 4, sp, tenpai))) { fold_flag[player - 1] = 0; }
					else { fold_flag[player - 1] = 1; tenpai[player - 1] = 0; }

					if (fold_flag[player - 1] == 0)
					{
						if (tenpai[player - 1] == 1 && pass_pro[player - 1][yama[jun]]<ra[5 * jun + 1])//
						{
							tmrr++;
							for (int ii = 0; ii<j / 4; ii++)
							{
								fv[ii] -= tktn[(player - 1) + 3 * (1 - sp[(player - 1)] + tatya[player].ippatu)][34];
								sim_num[ii]++;
							}
							j = (signed)yama.size();
							flag = 1;
						}
						else
						{
							int dis_pi = 0;
							dis_pi = yama[jun];

							int pl1 = 0, pl2 = 0;
							if (player == 1) { pl1 = 2; pl2 = 3; }
							else if (player == 2) { pl1 = 1; pl2 = 3; }
							else if (player == 3) { pl1 = 1; pl2 = 2; }

							if ((tenpai[pl1 - 1] * pass_pro[pl1 - 1][dis_pi] + (1 - tenpai[pl1 - 1]))*(tenpai[pl2 - 1] * pass_pro[pl2 - 1][dis_pi] + (1 - tenpai[pl2 - 1]))<ra[5 * jun + 2])//
							{
								for (int ii = 0; ii<j / 4; ii++)
								{
									sim_num[ii]++;
								}
								j = (signed)yama.size();
								flag = 1;
								yoko++;
							}
						}//
					}//
				}
				player = (player + 1) % 4;
			}//for(int j=0;j<turn;j++)

			if (flag == 0)
			{
				double ten = 0.0;
				if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 3) { ten = 0.0; }
				else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 2) { ten = -10.0; }
				else if (fold_flag[0] + fold_flag[1] + fold_flag[2] == 1) { ten = -15.0; }
				else { ten = -30.0; }

				for (int ii = 0; ii<18; ii++)
				{
					fv[ii] += ten + (double)game_field->ri_ti_bou / 4.0;
					sim_num[ii]++;
				}
				ryu_num++;
			}
			tmpi += tenpai[0] + tenpai[1] + tenpai[2];
			fold_aa += fold_flag[0] + fold_flag[1] + fold_flag[2];
			yama.clear();
			ra.clear();
			ura.clear();
			sute.clear();
			game_field->nokori_maisuu = cp_nokori;
		}//while(clock()-start<=1000)

		 //cout<<tktn[0][34]<<" "<<tktn[1][34]<<" "<<tktn[2][34]<<" "<<tktn[3][34]<<" "<<tktn[4][34]<<" "<<tktn[5][34]<<endl;
		 //	cout<<tmpi<<" "<<fold_aa<<endl;
		 //cout<<tmrr<<" "<<yoko<<" "<<ryu_num<<endl;
		for (int ii = 0; ii<18; ii++)
		{
			//cout<<fv[ii]<<" "<<sim_num[ii]<<endl;

			if (sim_num[ii]>0) { fv[ii] /= (double)sim_num[ii]; }
			else { fv[ii] = 0.0; }
			fv_pr[ii] = (double)sim_num[ii] / (double)sim_num[0];
			//if(tatya[2].ri_ti_flag==1){cout<<fv_pr[ii]<<endl;}
		}

		game_field->nokori_maisuu = cp_nokori;
		game_field->ri_ti_bou = cp_ribou;
		for (int i = 1; i<4; i++)
		{
			tatya[i].ippatu = cp_ippatu[i - 1];
		}
		return 0.0;
	}
};
#endif
