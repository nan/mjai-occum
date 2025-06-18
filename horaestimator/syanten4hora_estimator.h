
#pragma once
#ifndef __SYANTEN_MJHoraEstimatorShanten
#define __SYANTEN_MJHoraEstimatorShanten

class Syanten4HoraEstimator{
    
    int tehai[39];
    int mentu;
    int toitu;
    int kouho;
    int fuurosuu;
    int temp;
    int syanten_normal;
    
public:
    Syanten4HoraEstimator(){fuurosuu=0;};
    
    void set_fuurosuu(int a){fuurosuu=a;}
    
    void clear()
    {
        for(int i=0;i<38;i++){
            tehai[i]=0;
        }
        tehai[38]=-1;
        fuurosuu=0;
    }
    
    void set_tehai(int t[])
    {
        for(int i=0;i<38;i++){
            tehai[i]=t[i];
        }
        tehai[38]=-1;
    }
    
    int KokusiSyanten()
    {
        int kokusi_toitu=0,syanten_kokusi=13,i;
        
        for(i=1;i<30;i++){
            if(i%10==1||i%10==9||i%20==1||i%20==9){
                if(tehai[i])
                    syanten_kokusi--;
                if(tehai[i] >=2 && kokusi_toitu==0)
                    kokusi_toitu=1;
            }
        }
        
        for(i=31;i<38;i++){
            if(tehai[i]){
                syanten_kokusi--;
                if(tehai[i] >=2 && kokusi_toitu==0)
                    kokusi_toitu=1;
            }
        }
        
        syanten_kokusi-= kokusi_toitu;
        return syanten_kokusi;
    }
    
    
    int TiitoituSyanten()
    {
        int i=1,toitu=0,syurui=0,syanten_tiitoi;
        for(;i<=37;i++){
            for(;!tehai[i];i++);
            if(i>=38) continue;
            syurui++;
            if(tehai[i] >=2)
                toitu++;
        }
        syanten_tiitoi=6-toitu;
        if(syurui<7)
            syanten_tiitoi+= 7-syurui;
        return syanten_tiitoi;
    }
    
    int NormalSyanten()
    {
        mentu=0;
        toitu=0;
        kouho=0;
        temp=0;
        syanten_normal=8;
        for(int i=1;i<38;i++)
        {
            if(2 <= tehai[i])
            {
                toitu++;
                tehai[i] -= 2;
                mentu_cut(1);
                tehai[i] += 2;
                toitu--;
            }
        }
        if(fuurosuu == 0)
            mentu_cut(1);
        
        bool ji=false;
        return syanten_normal-fuurosuu*2+ji;
    }
    void mentu_cut(int i)
    {
        for(;!tehai[i];i++);
        if(i>=38){taatu_cut(1);return;}
        if(tehai[i]>=3)
        {
            mentu++;
            tehai[i]-=3;
            mentu_cut(i);
            tehai[i]+=3;
            mentu--;
        }
        if(tehai[i+1]&&tehai[i+2]&&i<30)
        {
            mentu++;
            tehai[i]--,tehai[i+1]--,tehai[i+2]--;
            mentu_cut(i);
            tehai[i]++,tehai[i+1]++,tehai[i+2]++;
            mentu--;
        }
        mentu_cut(i+1);
    }
    
    void taatu_cut(int i)
    {
        for(;!tehai[i];i++);
        if(i>=38)
        {
            temp=8-mentu*2-kouho-toitu;
            if(temp<syanten_normal) { syanten_normal=temp; }
            return;
        }
        if(mentu+kouho<4)
        {
            if(tehai[i]==2)
            {
                kouho++;
                tehai[i]-=2;
                taatu_cut(i);
                tehai[i]+=2;
                kouho--;
            }
            
            if(tehai[i+1]&&i<30)
            {
                kouho++;
                tehai[i]--,tehai[i+1]--;
                taatu_cut(i);
                tehai[i]++,tehai[i+1]++;
                kouho--;
            }
            
            if(tehai[i+2]&&i<30&&i%10<=8)
            {
                kouho++;
                tehai[i]--,tehai[i+2]--;
                taatu_cut(i);
                tehai[i]++,tehai[i+2]++;
                kouho--;
            }
        }
        taatu_cut(i+1);
    }
};
#endif
