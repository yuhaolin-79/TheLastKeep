struct ResourceResult {
    int code = 200;
    QString message = "操作成功";
    QList<QString> failResList;
};

struct PixmapCacheItem {
    QString resPath;
    QPixmap pixmap;
    int width = 0;
    int height = 0;
    bool loaded = false;
};

class ResourceManager {
public:
    ResourceResult preloadAll();
    QPixmap getPixmap(const QString& resPath, int targetW = 0, int targetH = 0);
    void clearAll();

private:
    QMap<QString, QPixmap> pixmapCache;
    QMap<QString, QSoundEffect*> soundCache;
    QPixmap placeholderPixmap;

private:
    QString makePixmapKey(const QString& resPath, int targetW, int targetH) const {
        return resPath + "#" + QString::number(targetW) + "x" + QString::number(targetH);
    }
};

enum class GridType {
    Empty = 0,      // 普通空地
    Road = 1,       // 敌人路径
    Castle = 2,     // 要塞终点
    Entrance = 3    // 敌人入口
};

enum class BuildState {
    CanBuild = 1,
    RoadForbidden = 2,
    CastleForbidden = 3,
    Occupied = 4,
    OutOfMap = 5
};

struct GridPos {
    int row = -1;
    int col = -1;
    bool valid = false;
};

struct MapGrid {
    int row = 0;
    int col = 0;
    GridType type = GridType::Empty;
    bool buildable = true;
    bool occupied = false;
    QRectF rect;
};

struct BuildCheckResult {
    int code = 200;
    BuildState buildState = BuildState::CanBuild;
    int row = -1;
    int col = -1;
};

enum class TowerType {
    Archer = 1,
    Mage = 2
};

enum class EnemyType {
    Goblin = 1,
    Orc = 2,
    Knight = 3
};

struct TowerObject {
    int towerId = 0;
    TowerType type = TowerType::Archer;

    QPointF pos;
    int gridRow = 0;
    int gridCol = 0;

    int damage = 10;
    float attackRange = 150.0f;
    float attackInterval = 1.0f;
    float cooldown = 0.0f;

    int cost = 50;
    bool active = true;
};

struct EnemyObject {
    int enemyId = 0;
    EnemyType type = EnemyType::Goblin;

    QPointF pos;
    int maxHp = 100;
    int currentHp = 100;

    float speed = 60.0f;
    int currentWaypointIndex = 0;

    int rewardGold = 10;
    int scoreValue = 10;
    bool alive = true;
    bool reachedCastle = false;
};

struct BulletObject {
    int bulletId = 0;

    QPointF pos;
    QPointF velocity;

    int damage = 10;
    float speed = 300.0f;
    int targetEnemyId = -1;

    bool active = true;
};

enum class CardType {
    DamageUp,
    BuildCostDown,
    GoldRewardUp,
    RangeUp
};

struct CardInfo {
    CardType type;
    QString name;
    QString description;
    float value = 0.0f;
    QString iconPath;
};

struct BuffState {
    float damageRate = 1.0f;       // 伤害倍率
    float buildCostRate = 1.0f;    // 建造价格倍率，小于1表示降价
    float goldRewardRate = 1.0f;   // 金币奖励倍率
    float attackRangeRate = 1.0f;  // 攻击范围倍率
};

inline void applyCardBuff(BuffState& buff, CardType type) {
    switch (type) {
    case CardType::DamageUp:
        buff.damageRate += 0.15f;
        break;
    case CardType::BuildCostDown:
        buff.buildCostRate *= 0.90f;
        break;
    case CardType::GoldRewardUp:
        buff.goldRewardRate += 0.20f;
        break;
    case CardType::RangeUp:
        buff.attackRangeRate += 0.10f;
        break;
    }
}