//
// Created by Hugo JEANNIN on 5/23/17.
//

#ifndef ANIMATION_HPP
# define ANIMATION_HPP

#define GLM_ENABLE_EXPERIMENTAL

# include <iostream>
# include <GLM/glm.hpp>
# include <GLM/ext.hpp>
# include <GLM/gtc/matrix_transform.hpp>
# include  <GLM/gtx/string_cast.hpp>
# include <GLFW/glfw3.h>
# include "Constant.hpp"
# include <stack>
# include <vector>
# include <cstdarg>
# include <map>

enum    transf_type
{
    TRANS,
    ROT,
    SCALE
};


class AnimationManager
{
public:

    typedef struct      s_transf
    {
        double              duration;
        int                 type;
        glm::mat4           matrix;
        std::string         name;
    }                   t_transf;

    typedef struct      s_model_attrib
    {
        bool                is_rendered;
        int                 pos_id;
        int                 end_pos_id;
        std::string         name;
        int                 model_id;
        GLuint              texture_id;
        glm::mat4           end_formation;
        glm::mat4           init_mat;
        glm::mat4           trans_mat;
        glm::mat4           rot_mat;
        glm::mat4           scale_mat;
        glm::mat4           matrix;
    }                   t_model_attrib;

    typedef struct      s_transf_serie
    {
        bool                            is_active;
        double                          serie_start_timer; /// time from now to start
        double                          serie_start_time; /// start timestamp
        double                          serie_end_timer; /// time from now to end
        double                          serie_end_time; /// end timestamp
        std::vector<t_model_attrib *>   models;
        std::vector<t_transf *>         serie;
        std::vector<double>             start_timing;
    }                   t_transf_serie;

    std::vector<t_model_attrib>         v_model_attrib;
    std::vector<t_transf_serie>         v_transf_serie; // TS
    std::vector<t_transf_serie *>       ensemble1;


    /// This remember individual transformation, that are not yet in a serie.
    std::vector<t_transf *> origin_transf_ptr;

    AnimationManager();
    ~AnimationManager();

    void init();
    void loadMinimal();
    void loadTruc();

    /// Use in order for a full Animation.
    void buildCustomModelAttrib(int amount, std::string name, int model_id, GLuint texture_id, glm::mat4 init_matrix = glm::mat4(1.0f));
    void addEmptyTS(int amount);
    void createNewTransformation(double duration,
                                 int type,
                                 glm::mat4 matrix,
                                 std::string name);
    void buildTS(t_transf_serie *transf_serie, int size, ...);
    void setTSIndividualStartTimings(t_transf_serie *transf_serie, int size, ...);
    void linkTS(t_transf_serie *transf_serie, t_model_attrib *model_attrib);
    void linkTS(int TS_ID, int ma_ID);
    void setTSGlobalDuration(t_transf_serie *transf_serie, double end_time, double delay = 0);

    /// Forget about that, under the hood.
    void applyEndFormation(t_transf_serie &transf_serie);
    void activateEnsemble(std::vector<t_transf_serie *> &ensemble);
    void stopAllAnimation();
    void stopSerie(t_transf_serie &transf_serie);
    void clearTSsModels(int start, int end);
    void update();

    /// Actual animation.
    void clearEverything();

    void sceneMinimal();
    void animMinimal();
    void animMinimal2();
    void applyMinimalStep1();
    void applyMinimalStep2();

    void sceneTruc();

    void printModelAttribPosID();
};

#endif

    ///////////////////////
    /// ANIMATION ORDER ///
    ///////////////////////
    /// scene:
    ///     buildCustomModelAttrib()
    /// anim:
    ///     addEmptyTransfSerie()
    ///     createNewTransformation()
    ///     buildTransformationSerie()
    ///     setTransformationSerieTimings()
    ///     setTransformationSerieTimers()
    /// apply:
    ///     clearTransfSeriesModels()
    ///     linkTransformationSerie()
    ///     v_transf_serie[X].models[X]->end_formation
    ///     ensemble1.clear()
    ///     ensemble1.push_back()
    ///     activateEnsemble()
    /////////////////////////


/*	OpenGL mat4:

	s = scale,   t = translation,	a = axis

[	sX		0		0		tX	]

[	0		sY		0		tY	]

[	0		0		sZ		tZ	]

[	0		0		0		tW	]

	|		|		|
	aX		aY		aZ
*/
